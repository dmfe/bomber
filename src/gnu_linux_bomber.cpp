/*-----------------------------------------------------------------------------
* File Name: gnu_linux_bomber.cpp
* Purpose:
* Creation Date: 12-01-2020
* Last Modified: Mon 13 Jan 2020 01:27:05 AM MSK
* Created by: dima
-----------------------------------------------------------------------------*/
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Display *d;
    int s;
    Window w;
    XEvent e;

    /* Open connection with the server*/
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }
    s = DefaultScreen(d);

    /* Create Window*/
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 500, 500, 1,
                            BlackPixel(d, s), WhitePixel(d, s));

    /* Process Window Close Event through event handler
       so XNextEvent doesn't fail*/
    Atom delWindow = XInternAtom(d, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(d, w, &delWindow, 1);

    /* Select kind of events we are interested in*/
    XSelectInput(d, w, ExposureMask|KeyPressMask);

    /* Map (show) the window*/
    XMapWindow(d, w);

    /* Event loop*/
    while(1) {
        XNextEvent(d, &e);

        /* Draw or redraw the window*/
        if (e.type == Expose) {
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
        }

        if (e.type == KeyPress) {
            /* Exit on ESC*/
            if (e.xkey.keycode == 0x09) {
                break;
            }
        }

        /* Handle Windows Close Event*/
        if (e.type == ClientMessage) {
            break;
        }
    }

    XDestroyWindow(d, w);

    XCloseDisplay(d);

    return(0);
}
