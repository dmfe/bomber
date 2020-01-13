/*-----------------------------------------------------------------------------
* File Name: gnu_linux_bomber.cpp
* Purpose:
* Creation Date: 12-01-2020
* Last Modified: Tue 14 Jan 2020 01:26:06 AM MSK
* Created by: dima
-----------------------------------------------------------------------------*/
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
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
                                        10, 10, 500, 500, 1,
                                        blackColor, whiteColor);

    /* Process Window Close Event through event handler
       so XNextEvent doesn't fail*/
    Atom delWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &delWindow, 1);

    /* Select kind of events we are interested in*/
    XSelectInput(display, window, ExposureMask|KeyPressMask);

    /* Map (show) the window*/
    XMapWindow(display, window);

    XEvent event;
    while(1) {
        XNextEvent(display, &event);

        /* Draw or redraw the window*/
        if (event.type == Expose) {
            XFillRectangle(display, window, DefaultGC(display, screenNumber),
                           20, 20, 10, 10);
        }

        if (event.type == KeyPress) {
            /* Exit on ESC*/
            if (event.xkey.keycode == 0x09) {
                break;
            }
        }

        /* Handle Windows Close Event*/
        if (event.type == ClientMessage) {
            break;
        }
    }

    XDestroyWindow(display, window);

    XCloseDisplay(display);

    return(0);
}
