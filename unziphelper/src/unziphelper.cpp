#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
        #ifndef __USE_FILE_OFFSET64
                #define __USE_FILE_OFFSET64
        #endif
        #ifndef __USE_LARGEFILE64
                #define __USE_LARGEFILE64
        #endif
        #ifndef _LARGEFILE64_SOURCE
                #define _LARGEFILE64_SOURCE
        #endif
        #ifndef _FILE_OFFSET_BIT
                #define _FILE_OFFSET_BIT 64
        #endif
#endif

#ifdef __APPLE__
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftello(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
# include <direct.h>
# include <io.h>
#else
# include <unistd.h>
# include <utime.h>
#endif

#include <unzip.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifdef _WIN32
#define USEWIN32IOAPI
#include <iowin32.h>
#endif

#include <unziphelper/unziphelper.h>

namespace  {
int do_extract_currentfile( unzFile uf )
{
    char filename_inzip[256];
    char* filename_withoutpath;
    char* p;
    int err=UNZ_OK;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;

    unz_file_info64 file_info;
    err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err!=UNZ_OK)
    {
        printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
        return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
    }
    else
    {
        const char* write_filename;
        int skip=0;

        write_filename = filename_withoutpath;

        err = unzOpenCurrentFilePassword(uf, nullptr);
        if (err!=UNZ_OK)
        {
            printf("error %d with zipfile in unzOpenCurrentFilePassword\n",err);
        }

        if ((skip==0) && (err==UNZ_OK))
        {
            fout=FOPEN_FUNC(write_filename,"wb");
            if (fout==NULL)
            {
                printf("error opening %s\n",write_filename);
            }
        }

        if (fout!=NULL)
        {
            printf(" extracting: %s\n",write_filename);

            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err<0)
                {
                    printf("error %d with zipfile in unzReadCurrentFile\n",err);
                    break;
                }
                if (err>0)
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        printf("error in writing extracted file\n");
                        err=UNZ_ERRNO;
                        break;
                    }
            }
            while (err>0);
            if (fout)
                    fclose(fout);
        }

        if (err==UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzCloseCurrentFile\n",err);
            }
        }
        else
            unzCloseCurrentFile(uf); /* don't lose the error */
    }

    free(buf);
    return err;
}

int do_extract_onefile(unzFile uf, const char* filename)
{
    if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK) {
        printf("file %s not found in the zipfile\n",filename);
        return 2;
    }

    if (do_extract_currentfile(uf) == UNZ_OK)
        return 0;
    else
        return 1;
}

unzFile openZipfile( const char *zipfilename )
{
    unzFile uf=NULL;
    char filename_try[MAXFILENAME+16] = "";

    if (zipfilename!=NULL) {

#        ifdef USEWIN32IOAPI
        zlib_filefunc64_def ffunc;
#        endif

        strncpy(filename_try, zipfilename,MAXFILENAME-1);
        /* strncpy doesnt append the trailing NULL, of the string is too long. */
        filename_try[ MAXFILENAME ] = '\0';

#        ifdef USEWIN32IOAPI
        fill_win32_filefunc64A(&ffunc);
        uf = unzOpen2_64(zipfilename,&ffunc);
#        else
        uf = unzOpen64(zipfilename);
#        endif
        if (uf==NULL) {
            strcat(filename_try,".zip");
#            ifdef USEWIN32IOAPI
            uf = unzOpen2_64(filename_try,&ffunc);
#            else
            uf = unzOpen64(filename_try);
#            endif
        }
    }

    if (uf==NULL) {
        printf("Cannot open %s or %s.zip\n",zipfilename,zipfilename);
        return uf;
    }
    printf("%s opened\n",filename_try);
    return uf;
}

} // namespace

namespace UnzipHelper {

int extract( const char *zipfilename, const char *filename_to_extract )
{
    const auto uf = openZipfile( zipfilename );
    if( uf != NULL ) {
        const auto result = do_extract_onefile( uf, filename_to_extract );
        unzClose( uf );
        return result;
    }
    else {
        return 1;
    }
}

std::vector<std::string> getFileList( const char *zipfilename )
{
    std::vector<std::string> files;
    const auto uf = openZipfile( zipfilename );
    if( uf == NULL ) {
        return files;
    }

    uLong i;
    unz_global_info64 gi;
    int err;

    err = unzGetGlobalInfo64(uf,&gi);
    if (err!=UNZ_OK)
        printf("error %d with zipfile in unzGetGlobalInfo \n",err);

    for (i=0;i<gi.number_entry;i++) {
        char filename_inzip[256];
        unz_file_info64 file_info;
        err = unzGetCurrentFileInfo64(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
        if (err!=UNZ_OK) {
            printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
            break;
        }

        files.push_back( filename_inzip );

        if ((i+1)<gi.number_entry) {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK) {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    unzClose( uf );
    return files;
}

} // namespace UnzipHelper
