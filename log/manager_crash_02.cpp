#0  0x0000003a5f079ba5 in _int_malloc () from /lib64/libc.so.6
#1  0x0000003a5f07ab1c in malloc () from /lib64/libc.so.6
#2  0x00007ffff63423f0 in QArrayData::allocate(unsigned long, unsigned long, unsigned long, QFlags<QArrayData::AllocationOption>) () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#3  0x00007ffff63c2583 in QString::QString(int, Qt::Initialization) () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#4  0x00007ffff655c3ed in ?? () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#5  0x00007ffff63c8ade in QString::fromUtf8_helper(char const*, int) () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#6  0x00007ffff63c9504 in QString::compare_helper(QChar const*, int, char const*, int, Qt::CaseSensitivity) () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#7  0x000000000040e4cb in QString::operator== (this=0x7fff961fbbd8, s=0x44f45c "false") at /opt/Qt5.7.1/5.7/gcc_64/include/QtCore/qstring.h:1174
#8  0x00000000004302f1 in tcp_socket<manager>::run (this=0x6fb930) at /opt/cats_farm/code/manager/../utils/tcp.h:114
#9  0x00007ffff6341539 in ?? () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#10 0x0000003a5f407aa1 in start_thread () from /lib64/libpthread.so.0
#11 0x0000003a5f0e8c4d in clone () from /lib64/libc.so.6
