Import('env')
env.Replace(FUSESCMD="avrdude $UPLOADERFLAGS -e -Ulock:w:0x3F:m -Uhfuse:w:0xDE:m -Uefuse:w:0x05:m -Ulfuse:w:0xFF:m")
