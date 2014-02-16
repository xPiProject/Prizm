typedef unsigned short FONTCHARACTER;

int Bfile_CloseFile_OS( int HANDLE );
int Bfile_CreateEntry_OS( const unsigned short*filename, int mode, int*size );
int Bfile_DeleteEntry( const unsigned short *filename );
int Bfile_OpenFile_OS( const unsigned short*filename, int mode );
int Bfile_SeekFile_OS( int handle, int pos );
int Bfile_WriteFile_OS( int HANDLE, const void *buf, int size );