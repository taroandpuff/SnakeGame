aux_source_directory(. SRC_LIST)

add_executable(SnakeGame ${SRC_LIST} ../Headers/widget.h)

# 指定可执行文件链接时需要依赖的库文件
#target_link_libraries(ChatServer muduo_net muduo_base mysqlclient hiredis pthread) 
target_link_libraries(SnakeGame Qt5::Widgets)