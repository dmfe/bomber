/*-----------------------------------------------------------------------------
* File Name: gnu_linux_bomber.cpp
* Purpose:
* Creation Date: 12-01-2020
* Last Modified: Thu 16 Jan 2020 02:55:57 AM MSK
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

void SetWindowManagerHints(Display *display,
                                  char *PClass,
                                  char *argv[],
                                  int argc,
                                  Window window,
                                  int x,
                                  int y,
                                  int width,
                                  int height,
                                  int minWidth,
                                  int minHeight,
                                  char *title,
                                  char *iconTitle,
                                  Pixmap pixmap) {

    XTextProperty windowName;
    XTextProperty iconName;
    if (!XStringListToTextProperty(&title, 1, &windowName) ||
        !XStringListToTextProperty(&iconTitle, 1, &iconName)) {

        fprintf(stderr, "Error during creating text properties.\n");
        exit(1);
    }

    XSizeHints sizeHints = {};
    sizeHints.flags = PPosition|PSize|PMinSize;
    sizeHints.x = x;
    sizeHints.y = y;
    sizeHints.width = width;
    sizeHints.height = height;
    sizeHints.min_width = minWidth;
    sizeHints.min_height = minHeight;

    XWMHints wmHints = {};
    wmHints.flags = StateHint|IconPixmapHint|InputHint;
    wmHints.initial_state = NormalState;
    wmHints.input = True;
    wmHints.icon_pixmap = pixmap;

    XClassHint classHints = {};
    classHints.res_name = argv[0];
    classHints.res_class = PClass;

    XSetWMProperties(display, window, &windowName, &iconName, argv, argc,
                     &sizeHints, &wmHints, &classHints);
}

void handleExposeEvent(Display *display,
                       Window window,
                       XEvent *event,
                       int black,
                       int white,
                       GC gc) {

    if (!event->xexpose.count) {
        fprintf(stdout, "Window redraw\n");

        static int color = black;
        if (color == black) {
           color = white;
        } else {
            color = black;
        }

        XWindowAttributes winAttrs;
        XGetWindowAttributes(display, window, &winAttrs);

        XSetForeground(display, gc, color);
        XFillRectangle(display, window, gc, 0, 0,
                       winAttrs.width, winAttrs.height);
        XFlush(display);
    }
}

int main(int argc, char *argv[]) {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int screenNumber = DefaultScreen(display);
    int blackColor = BlackPixel(display, screenNumber);
    int whiteColor = WhitePixel(display, screenNumber);

    Window window = XCreateSimpleWindow(display,
                                        RootWindow(display, screenNumber),
                                        X, Y, WIDTH, HEIGHT, BORDER_WIDTH,
                                        blackColor, whiteColor);

    SetWindowManagerHints(display, PRG_CLASS, argv, argc, window,
                          X, Y, WIDTH, HEIGHT, WIDTH_MIN, HEIGHT_MIN,
                          TITLE, ICON_TITLE, 0);

    /* Process Window Close Event through event handler
       so XNextEvent doesn't fail*/
    Atom delWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &delWindow, 1);

    /* Select kind of events we are interested in*/
    XSelectInput(display, window, ExposureMask|KeyPressMask);

    /* Map (show) the window*/
    XMapWindow(display, window);

    XEvent event;
    GC gc = XCreateGC(display, window, 0, NULL);
    while(1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            fprintf(stdout, "Window exposed\n");
            handleExposeEvent(display, window, &event,
                              blackColor, whiteColor, gc);
        }

        if (event.type == KeyPress) {
            fprintf(stdout, "Key pressed\n");
            if (event.xkey.keycode == 0x09) {
                break;
            }
        }

        /* Handle Windows Close Event*/
        if (event.type == ClientMessage) {
            fprintf(stdout, "window cloase event\n");
            break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return(0);
}
