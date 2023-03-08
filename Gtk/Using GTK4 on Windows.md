# GTK4를 Windows OS에서 사용하는 방법

크게 두 가지가 있다.  
하나는 VS CODE + CMake + gcc(or clang)을 이용하는 방법.  
나머지 하나는 Visual Studio + vcpkg를 이용하는 방법.  
두 방식 모두 MSYS2는 설치를 해야 한다.  
현재 vcpkg는 GTK에 대한 업데이트를 하지 않고 있기에 최신 GTK를 이용하기 쉬운 CMake를 사용하는 방법을 알아보자.  
&nbsp;  

## VS Code를 통한 방법  

일단 64-bit 프로그램을 gcc 컴파일러를 사용하여 개발하는 것을 전제로 다루겠다.  
32-bit 프로그램을 개발하고자 한다면 명령어의 ```x86_64``` 부분이 ```i686```으로, ```mingw64``` 부분이 ```mingw32```로 교체되면 된다.  
clang 컴파일러를 사용한다면 clang 전용 gtk4 패키지가 존재하니 MSYS2 패키지에서 이름을 검색해 설치하자. (아마 ```mingw-w64-clang-x86_64-gtk4```일 것이다.)  

### 빌드 과정  

1. MSYS2 설치  
    MSYS2를 ```C:\msys64``` 경로를 만들고 거기에 설치한다. (설치 링크: https://www.msys2.org/)  

2. MSYS2를 실행  
    콘솔창에 ```사용자이름@데스크탑 이름 MSYS ~``` 이렇게 뜨고 커서가 깜박여야 정상이다.  

3. MSYS2 초반 세팅  
    ```pacman -Syu``` 수행 후 MSYS2 재시작하고 ```pacman -Su``` 수행하고 ```pacman -Syu``` 수행한다.  

4. gcc 설치  
    gcc를 사용할 거면 ```pacman -S mingw-w64-x86_64-gcc```를 수행한다.  

5. gtk 설치  
    gcc 사용이라면 ```pacman -S mingw-w64-x86_64-gtk4```를 수행한다.  

6. pkg-config 설치  
    ```pacman -S pkg-config```를 수행한다.  

7. GUI 툴 킷인 Glade 설치   
    Glade는 GTK의 UI 편집기이다.  
    ```pacman -S mingw-w64-x86_64-glade```를 수행하여 설치해주자.  

8. toolchain 설치  
    ```pacman -S --needed base-devel mingw-w64-x86_64-toolchain```를 수행하여 GDB와 같은 toolchain을 설치해준다.  

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
    밑 코드를 붙여넣고 빌드해보자.  
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
    Hello World 버튼을 누르면 창을 닫는 예제이다.  
    잘 모르겠다면 레포지토리 같은 위치에 있는 VS Code 프로젝트인 Gtk4.Sample를 참고해보자.  

### 배포 과정  

1. 실행 파일 확인  
    빌드 과정이 끝나면 특정 exe 실행 파일이 도출될 것이다.  
    여기선 편하게 MyGtk4App.exe라고 부르겠다.  

2. 앱 구동 환경 구성  
    특정 폴더를 만든다.  
    아무 이름이나 상관없지만 필자는 Gtk4App이라는 폴더를 만들겠다.  
    Gtk4App 폴더 내부에 bin, etc, lib, share 폴더를 생성한다. (여기선 폴더 이름이 중요하다.)  
    Gtk4App/bin 폴더 내부에 MyGtk4App.exe를 넣는다.  

3. 실행 dll 복사  
    MSYS2를 관리자 모드로 열고 ```cd c:/msys64/mingw64/bin```를 통해 gtk4-demo.exe 앱이 있는 경로로 이동한다.  
    ```mkdir ./dlls```로 dlls 폴더를 만들어준다.  
    그 후 ```ldd gtk4-demo.exe | grep '\/mingw.*\.dll' -o | xargs -I{} cp "{}" ./dlls```로 필요 dll 파일을 dlls 폴더에 옮겨준다.  
    dlls 폴더 내부에 있는 dll 파일들을 Gtk4App/bin/MyGtk4App.exe 위치에 떨궈준다. (이후 dlls 폴더는 삭제해도 된다.)  

4. setting.ini 생성  
    Gtk4App/etc 내부에 gtk-4.0 폴더를 생성한다.  
    Gtk4App/etc/gtk-4.0 내부에 setting.ini를 생성한다.  
    자신의 앱이 사용할 환경대로 setting.ini를 작성해준다.  

5. 리소스 라이브러리 설정  
    gtk4에서 이미지나 영상등을 편하게 읽어오기 위한 라이브러리의 경로를 지정해줘야 한다.  
    c:/msys64/mingw64/lib 경로에 보면 gdk-pixbuf-2.0 라는 폴더가 있을텐데 이 녀석을 복사하여 Gtk4App/lib 폴더 내부에 붙여넣어 준다.  
    Gtk4App/lib/gdk-pixbuf-2.0/2.10.0 내부를 보면 loaders.cache 파일이 있을텐데 이 녀석을 열어 편집해야 한다.  
    loaders.cache 내용 중 ```# LoaderDir = C:\msys64\mingw64\lib\gdk-pixbuf-2.0\2.10.0\loaders``` 이 부분을 ```# LoaderDir = ..\lib\gdk-pixbuf-2.0\2.10.0\loaders``` 이렇게 바꿔준다.  

6. schemas 설정  
    c:/msys64/mingw64/share 경로에 보면 glib-2.0 폴더가 있을 텐데 이 녀석을 복사하여 Gtk4App/share 폴더 내부에 붙여넣어 준다.  
    Gtk4App/share/glib-2.0 내부에 보면 schemas 폴더가 있을텐데 schemas 폴더만 남기고 다 지워준다.  
    MSYS2를 켜고 ```cd c:/msys64/mingw64/bin```를 통해 glib-compile-schemas.exe 앱이 있는 경로로 이동한다.  
    ```./glib-compile-schemas.exe <Gtk4App/share/glib-2.0/schemas 절대 경로>```를 통해 gschemas.compiled 파일을 Gtk4App\share\glib-2.0\schemas 내부에 생성해줘야 한다. (ex. ./glib-compile-schemas.exe D:/Gtk4App/share/glib-2.0/schemas)  

7. 아이콘, 테마 설정  
    c:/msys64/mingw64/share 경로에 보면 icons, themes 폴더가 있을텐데 이 녀석들을 복사하여 Gtk4App/share 폴더 내부에 붙여넣어 준다.  
    앱의 아이콘을 변경해주려면 Gtk4App/share/icons/hicolor/[특정 해상도]/apps 내부 파일을 수정해주면 된다.  
    특정 테마를 적용하려면 Gtk4App/share/themes 폴더 내부에 설정할 테마를 넣고 Gtk4App/etc/gtk-4.0 내부에 setting.ini 파일로 설정해주면 된다.  

8. 최종 배포  
    Gtk4App 폴더를 압축하고 배포하면 된다. (사용자가 폴더 이름을 따로 지었다면 그 폴더를 압축하면 된다.)  
    배포 구성은 레포지토리 같은 위치에 있는 Gtk4.App을 참고하면 이해하기 쉬울 것이다.  
&nbsp;  

출처  
- https://discourse.gnome.org/t/packaging-gtk-on-windows-documentation-suggestions/213/3  
- https://gist.github.com/mjakeman/0add69647a048a5e262d591072c7facb  