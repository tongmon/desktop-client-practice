# GTK4를 Windows OS에서 사용하는 방법

크게 두 가지가 있다.  
하나는 VS CODE + CMake + gcc(or clang)을 이용하는 방법.  
나머지 하나는 Visual Studio + vcpkg를 이용하는 방법.  
두 방식 모두 MSYS2는 설치를 해야 한다.  
첫번째 방법부터 보자.  
&nbsp;  

## VS Code를 통한 방법  

일단 64-bit 프로그램 개발하는 것을 전제로 다루겠다.  
32-bit 프로그램을 개발하고자 한다면 명령어의 ```x86_64``` 부분이 ```i686```으로, ```mingw64``` 부분이 ```mingw32```로 교체되면 된다.  

1. MSYS2 설치  
    MSYS2를 ```C:\msys64``` 경로를 만들고 거기에 설치한다. (설치 링크: https://www.msys2.org/)  

2. MSYS2를 실행  
    콘솔창에 ```사용자이름@데스크탑 이름 MSYS ~``` 이렇게 뜨고 커서가 깜박여야 정상이다.  

3. MSYS2 초반 세팅  
    ```pacman -Syu``` 수행 후 MSYS2 재시작하고 ```pacman -Su``` 수행하고 ```pacman -Syu``` 수행한다.  

4. gcc(or clang) 설치  
    gcc를 사용할 거면 ```pacman -S mingw-w64-x86_64-gcc```를 수행한다.  
    clang을 사용할 거면 ```pacman -S clang```을 수행한다.  

5. gtk 설치  
    gcc 사용이라면 ```pacman -S mingw-w64-x86_64-gtk4```를 수행한다.  
    clang 사용이라면 ```pacman -S mingw-w64-clang-x86_64-gtk4```를 수행한다.  

6. pkg-config 설치  
    ```pacman -S pkg-config```를 수행한다.  

7. GUI 툴 킷인 Glade 설치  
    Glade는 GTK의 UI 편집기이다.  
    ```pacman -S mingw-w64-x86_64-glade```를 수행하여 설치해주자.  

8. toolchain 설치  
    gcc를 사용한다면 ```pacman -S --needed base-devel mingw-w64-x86_64-toolchain```를 수행하여 GDB와 같은 toolchain을 설치해준다.  

9. 환경 변수 설정  
    ```고급 시스템 설정 보기```에 들어가 고급 탭을 선택하고 환경 변수를 설정해야 한다.  
    사용자 변수 항목의 Path를 편집하여 ```C:\msys64\mingw64\bin``` 경로를 넣어주고 적용한다.  

10. CMakeLists.txt 편집  
    GTK4를 사용하고자 하는 프로젝트의 CMakeLists.txt를 편집해줘야 한다.  
    ```cmake
    # 윈도우 운영체제에서 GTK 앱을 구동시키면서 콘솔창을 같이 띄우지 않기 위함.
    set(CMAKE_CXX_FLAGS "-mwindows ${CMAKE_CXX_FLAGS}")

    # pkg-config를 이용해 설치된 gtk4 찾기  
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

    # gtk4 라이브러리 연동
    target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::GTK4)
    ```
    이러한 CMake 함수들을 적절히 프로젝트에 넣어주면 된다.  
    IMPORTED_TARGET을 사용하기에 따로 Include Directory를 지정해주지 않아도 된다.  

11. 데모 앱 실행  
    ```c++ 
    #include <gtk/gtk.h>

    static void print_hello(GtkWidget *widget,
                            gpointer data)
    {
        g_print("Hello World\n");
    }

    static void activate(GtkApplication *app,
                         gpointer user_data)
    {
        GtkWidget *window;
        GtkWidget *grid;
        GtkWidget *button;

        /* create a new window, and set its title */
        window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "Window");

        /* Here we construct the container that is going pack our buttons */
        grid = gtk_grid_new();

        /* Pack the container in the window */
        gtk_window_set_child(GTK_WINDOW(window), grid);

        button = gtk_button_new_with_label("Button 1");
        g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

        /* Place the first button in the grid cell (0, 0), and make it fill
         * just 1 cell horizontally and vertically (ie no spanning)
         */
        gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

        button = gtk_button_new_with_label("Button 2");
        g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

        /* Place the second button in the grid cell (1, 0), and make it fill
         * just 1 cell horizontally and vertically (ie no spanning)
         */
        gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

        button = gtk_button_new_with_label("Quit");
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);

        /* Place the Quit button in the grid cell (0, 1), and make it
         * span 2 columns.
         */
        gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

        gtk_widget_show(window);
    }

    int main(int argc, char **argv)
    {
        GtkApplication *app;
        int status;

        app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);

        return status;
    }
    ```
    위 코드를 붙여넣고 실행해보자.  