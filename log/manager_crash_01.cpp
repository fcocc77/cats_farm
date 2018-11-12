#0  0x0000003a5f079ada in _int_malloc () from /lib64/libc.so.6
#1  0x0000003a5f07ab1c in malloc () from /lib64/libc.so.6
#2  0x0000003a6a4bd0cd in operator new(unsigned long) () from /usr/lib64/libstdc++.so.6
#3  0x0000003a6a49c3c9 in std::string::_Rep::_S_create(unsigned long, unsigned long, std::allocator<char> const&) () from /usr/lib64/libstdc++.so.6
#4  0x0000003a6a49cde5 in ?? () from /usr/lib64/libstdc++.so.6
#5  0x0000003a6a49cf33 in std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(char const*, std::allocator<char> const&) () from /usr/lib64/libstdc++.so.6
#6  0x00000000004132df in nlohmann::basic_json<std::map, std::vector, std::string, bool, long, unsigned long, double, std::allocator, nlohmann::adl_serializer>::operator[]<char const> (this=0x7fffe402a4b8, key=0x44f062 "timer") at /opt/cats_farm/code/manager/../utils/json.h:12734
#7  0x000000000040bed8 in manager::struct_to_json (this=0x7fffffffebc0) at /opt/cats_farm/code/manager/manager.cpp:337
#8  0x000000000043d1aa in manager::container_save (this=0x7fffffffebc0) at /opt/cats_farm/code/manager/update.cpp:22
#9  0x000000000043d148 in manager::update_all (this=0x7fffffffebc0) at /opt/cats_farm/code/manager/update.cpp:7
#10 0x0000000000411dc6 in void threading<void (), manager>(void (manager::*)(), manager*)::{lambda()#1}::operator()() const (__closure=0x6def90) at /opt/cats_farm/code/manager/../utils/threading.h:25
#11 0x000000000042ad7f in QtPrivate::FunctorCall<QtPrivate::IndexesList<>, QtPrivate::List<>, void, void threading<void (), manager>(void (manager::*)(), manager*)::{lambda()#1}>::call({lambda()#1}&, void**) (f=..., arg=0x7fffed04cc80) at /opt/Qt5.7.1/5.7/gcc_64/include/QtCore/qobjectdefs_impl.h:135
#12 0x0000000000428b0b in QtPrivate::Functor<void threading<void (), manager>(void (manager::*)(), manager*)::{lambda()#1}, 0>::call<QtPrivate::List<>, void>({lambda()#1}&, void*, {lambda()#1}&*) (f=..., arg=0x7fffed04cc80) at /opt/Qt5.7.1/5.7/gcc_64/include/QtCore/qobjectdefs_impl.h:192
#13 0x000000000042666b in QtPrivate::QFunctorSlotObject<void threading<void (), manager>(void (manager::*)(), manager*)::{lambda()#1}, 0, QtPrivate::List<>, void>::impl(int, QtPrivate::QSlotObjectBase*, QObject*, void**, bool*) (which=1, this_=0x6def80, r=0x6deed0, a=0x7fffed04cc80, ret=0x0) at /opt/Qt5.7.1/5.7/gcc_64/include/QtCore/qobject_impl.h:169
#14 0x00007ffff652fe3d in QMetaObject::activate(QObject*, int, int, void**) () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#15 0x00007ffff6341520 in ?? () from /opt/Qt5.7.1/5.7/gcc_64/lib/libQt5Core.so.5
#16 0x0000003a5f407aa1 in start_thread () from /lib64/libpthread.so.0
#17 0x0000003a5f0e8c4d in clone () from /lib64/libc.so.6