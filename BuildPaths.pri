

CONFIG(win32-msvc*){

CONFIG(debug,debug|release){
BUILD_PATH_  = $$PWD/msvc_outs/bind
}else{
BUILD_PATH_  = $$PWD/msvc_outs/bin
}


}else{

CONFIG(debug,debug|release){
BUILD_PATH_  = $$PWD/outs/bind
}else{
BUILD_PATH_  = $$PWD/outs/bin
}

}






