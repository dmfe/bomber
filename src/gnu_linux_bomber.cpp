/*-----------------------------------------------------------------------------
* File Name: gnu_linux_bomber.cpp
* Purpose:
* Creation Date: 12-01-2020
* Last Modified: Fri 17 Jan 2020 04:48:49 AM MSK
* Created by: dima
-----------------------------------------------------------------------------*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 0
#define Y 0
#define WIDTH 400
#define HEIGHT 400
#define WIDTH_MIN 100
#define HEIGHT_MIN 100
#define BORDER_WIDTH 1
#define TITLE "Bomber"
#define ICON_TITLE "Bomber"
#define PRG_CLASS "Bomber"

#define internal static
#define local_persist static
#define global_variable static

//TODO: This is a global for now.
global_variable bool Running;

struct {
    int width;
    int height;
    char *title;
    char *icon_title;

    Display *display;
    int screen;
    Window root;
    Window window;
    XEvent *event;
    GC gc;
    unsigned long black_pixel;
    unsigned long white_pixel;
} app_win;

internal void
x_connect() {
    app_win.display = XOpenDisplay(NULL);
    if (!app_win.display) {
        fprintf(stderr, "Could not opent the display.\n");
        exit(1);
    }
    app_win.screen = DefaultScreen(app_win.display);
    app_win.root = RootWindow(app_win.display, app_win.screen);
    app_win.black_pixel = BlackPixel(app_win.display, app_win.screen);
    app_win.white_pixel = WhitePixel(app_win.display, app_win.screen);
}

internal void
create_window() {
    app_win.width = WIDTH;
    app_win.height = HEIGHT;
    app_win.window = XCreateSimpleWindow(app_win.display,
                                         app_win.root,
                                         X, Y,
                                         app_win.width,
                                         app_win.height,
                                         BORDER_WIDTH,
                                         app_win.black_pixel,
                                         app_win.white_pixel);

    app_win.title = TITLE;
    app_win.icon_title = ICON_TITLE;
    XTextProperty window_name;
    XTextProperty icon_name;
    if (!XStringListToTextProperty(&app_win.title, 1, &window_name) ||
        !XStringListToTextProperty(&app_win.icon_title, 1, &icon_name)) {

        fprintf(stderr, "Error during creating text properties.\n");
        exit(1);
    }

    XSetWMName(app_win.display, app_win.window, &window_name);
    XSetWMIconName(app_win.display, app_win.window, &icon_name);

    Atom del_window = XInternAtom(app_win.display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(app_win.display, app_win.window, &del_window, 1);

    XSelectInput(app_win.display, app_win.window,
                 ExposureMask|
                 KeyPressMask|
                 StructureNotifyMask);

    XMapWindow(app_win.display, app_win.window);
}

internal void
set_up_gc() {
    app_win.screen = DefaultScreen(app_win.display);
    app_win.gc = XCreateGC(app_win.display, app_win.window, 0, 0);
    XSetBackground(app_win.display, app_win.gc, app_win.white_pixel);
    XSetForeground(app_win.display, app_win.gc, app_win.black_pixel);
}

internal void
draw_screen() {
    if (!app_win.event->xexpose.count) {
        fprintf(stdout, "Redraw handling.\n");
        local_persist int color = app_win.black_pixel;
        if (color == app_win.black_pixel) {
           color = app_win.white_pixel;
        } else {
            color = app_win.black_pixel;
        }

        XSetForeground(app_win.display, app_win.gc, color);
        XFillRectangle(app_win.display, app_win.window, app_win.gc, 0, 0,
                       app_win.width, app_win.height);
        XFlush(app_win.display);
    }
}

internal void resizeDIBSection(int width, int height);

internal void
event_loop() {
    Running = true;
    while(Running) {
        XEvent event;
        XNextEvent(app_win.display, &event);
        app_win.event = &event;

        if (event.type == Expose) {
            draw_screen();
        }

        if (event.type == ConfigureNotify) {
            XConfigureEvent config_event = event.xconfigure;

            if (config_event.width != app_win.width ||
                config_event.height != app_win.height) {

                app_win.width = config_event.width;
                app_win.height = config_event.height;

                resizeDIBSection(app_win.width, app_win.height);
            }
        }

        if (event.type == KeyPress) {
            fprintf(stdout, "Key pressed.\n");
        }

        if (event.type == ClientMessage) {
            //TODO: Handle this with a message to the user.
            Running = false;
        }
    }
}

internal void
release() {
    XFreeGC(app_win.display, app_win.gc);
    XDestroyWindow(app_win.display, app_win.window);
    XCloseDisplay(app_win.display);
}

internal void
resizeDIBSection(int width, int height) {
    fprintf(stdout, "Resize handling.\n");
}

int main(int argc, char *argv[]) {
    x_connect();
    create_window();
    set_up_gc();
    event_loop();
    release();

    return(0);
}
