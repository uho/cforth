// Extension routines

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#ifdef __APPLE__
#include <stdlib.h>
#endif
#include "forth.h"
#include "sha256.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void ms(cell nms)
{
    usleep(nms*1000);  // nanosleep(timespec) would be better
}

void us(cell nus)
{
    usleep(nus);  // nanosleep(timespec) would be better
}

#include <sys/time.h>
cell get_msecs(void)
{
    struct timeval tv;
    unsigned int msecs;
    gettimeofday(&tv, NULL);
    msecs =  (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
    return (cell)msecs;
}

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void set_error_callback(void)
{
    glfwSetErrorCallback(error_callback);
}

void gl_triangle(int width, int height)
{
    float ratio;

    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
}

cell ((* const ccalls[])()) = {
  // OS-independent functions
  C(ms)                //c ms             { i.ms -- }
  C(get_msecs)         //c get-msecs      { -- i.ms }
  C(us)                //c us             { i.microseconds -- }

  C(glfwInit)          //c glfw-init           { -- i.okay }
  C(glfwTerminate)     //c glfw-terminate      { -- }
  C(set_error_callback)//c set-error-callback  { -- }
  C(glfwCreateWindow)  //c glfw-create-window  { a.share a.monitor $name i.h i.w -- a.window }
  C(glfwMakeContextCurrent) //c glfw-make-context-current  { a.window -- }
  C(glfwWindowShouldClose)  //c glfw-window-should-close   { a.window -- i.close? }
  C(glfwGetFramebufferSize) //c glfw-get-framebuffer-size  { a.height a.width a.window -- }
  C(glfwSwapBuffers)        //c glfw-swap-buffers          { a.window -- }
  C(glfwSwapInterval)       //c glfw-swap-interval         { i.interval -- }
  C(glfwPollEvents)         //c glfw-poll-events           { -- }
  C(glfwWindowHint)         //c glfw-window-hint           { i.value i.hint# -- }

  C(glewInit)               //c glew-init

#if 0
  C(glViewport)             //x gl-viewport      { i.height i.width i.y i.x -- }
  C(glClear)                //x gl-clear         { i.bits -- }
  C(glMatrixMode)           //x gl-matrix-mode   { i.mode -- }
  C(glLoadIdentity)         //x gl-load-identity { -- }
#endif
  C(gl_triangle)            //c gl-triangle      { i.height i.width -- }
};
