/*
The MIT License (MIT)

Copyright © 2021-2023 Antonio Dias (https://github.com/antonypro)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "macosnative.h"
#include <Cocoa/Cocoa.h>

void macOSNative::setStyle(long winid, StyleType type)
{
    NSView *nativeView = reinterpret_cast<NSView*>(winid);
    NSWindow *nativeWindow = [nativeView window];

    switch (type)
    {
    case StyleType::NoState:
    {
        [nativeWindow setStyleMask:NSWindowStyleMaskResizable |
                                   NSWindowStyleMaskMiniaturizable |
                                   NSWindowStyleMaskClosable |
                                   NSWindowStyleMaskTitled |
                                   NSWindowStyleMaskFullSizeContentView];
        [nativeWindow setMovableByWindowBackground:NO];
        [nativeWindow setMovable:NO];
        [nativeWindow setTitlebarAppearsTransparent:YES];
        [nativeWindow setShowsToolbarButton:NO];
        [nativeWindow setTitleVisibility:NSWindowTitleHidden];
        [nativeWindow standardWindowButton:NSWindowMiniaturizeButton].hidden = YES;
        [nativeWindow standardWindowButton:NSWindowCloseButton].hidden = YES;
        [nativeWindow standardWindowButton:NSWindowZoomButton].hidden = YES;
        [nativeWindow makeKeyWindow];

        break;
    }
    case StyleType::Disabled:
    {
        [nativeWindow setStyleMask:NSWindowStyleMaskFullSizeContentView];
        [nativeWindow setMovableByWindowBackground:NO];
        [nativeWindow setMovable:NO];
        [nativeWindow setTitlebarAppearsTransparent:YES];
        [nativeWindow setShowsToolbarButton:NO];
        [nativeWindow setTitleVisibility:NSWindowTitleHidden];
        [nativeWindow standardWindowButton:NSWindowMiniaturizeButton].hidden = YES;
        [nativeWindow standardWindowButton:NSWindowCloseButton].hidden = YES;
        [nativeWindow standardWindowButton:NSWindowZoomButton].hidden = YES;
        [nativeWindow makeKeyWindow];

        break;
    }
    case StyleType::Fullscreen:
    {
        [nativeWindow setStyleMask:0];

        break;
    }
    }
}

//\cond HIDDEN_SYMBOLS
@interface Handler : NSObject
{
}
@end
//\endcond

Handler *m_handler;

void macOSNative::registerNotification(const char *notification_name, long wid)
{
    NSView *nativeView = reinterpret_cast<NSView*>(wid);
    NSWindow *nativeWindow = [nativeView window];

    [[NSNotificationCenter defaultCenter]
    addObserver:m_handler
    selector:@selector(NotificationHandler:)
    name:[NSString stringWithUTF8String:notification_name]
    object:nativeWindow];
}

void macOSNative::unregisterNotification()
{
    [[NSNotificationCenter defaultCenter]
    removeObserver:m_handler];

    [m_handler release];
}

//\cond HIDDEN_SYMBOLS
@implementation Handler
+ (void)load
{
    m_handler = static_cast<Handler*>(self);
}

+(void)NotificationHandler:(NSNotification*)notification
{
    const NSString *str = [notification name];

    const NSWindow *nativeWindow = [notification object];

    const NSView *nativeView = [nativeWindow contentView];

    const char *cstr = [str cStringUsingEncoding:NSUTF8StringEncoding];

    macOSNative::handleNotification(cstr, long(nativeView));
}
@end
//\endcond

//\cond HIDDEN_SYMBOLS
@interface ThemeChangeHandler : NSObject
{
}
@end
//\endcond

ThemeChangeHandler *m_theme_change_handler;

//\cond HIDDEN_SYMBOLS
@implementation ThemeChangeHandler
+ (void)load
{
    m_theme_change_handler = static_cast<ThemeChangeHandler*>(self);
}

+(void)ThemeChangeNotification:(NSNotification*)notification
{
    const NSString *str = [notification name];

    const char *cstr = [str cStringUsingEncoding:NSUTF8StringEncoding];

    macOSNative::handleNotification(cstr, 0);
}
@end
//\endcond

void macOSNative::registerThemeChangeNotification()
{
    [[NSDistributedNotificationCenter defaultCenter]
    addObserver:m_theme_change_handler
    selector:@selector(ThemeChangeNotification:)
    name:@"AppleInterfaceThemeChangedNotification"
    object:nil];
}

const char *macOSNative::themeName()
{
    NSString *str = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
    const char *cstr = [str cStringUsingEncoding:NSUTF8StringEncoding];
    return cstr;
}
