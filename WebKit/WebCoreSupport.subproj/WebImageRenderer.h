/*	WebImageRenderer.h
	Copyright 2002, Apple, Inc. All rights reserved.
*/

#import <Cocoa/Cocoa.h>

@protocol WebCoreImageRenderer;

//#ifndef OMIT_TIGER_FEATURES
//#define USE_CGIMAGEREF YES
//#endif

#ifdef USE_CGIMAGEREF
@class WebImageData;

@interface WebImageRenderer : NSObject <WebCoreImageRenderer>
{
    NSString *MIMEType;

    WebImageData *imageData;

    NSRect targetAnimationRect;
    
    NSSize adjustedSize;
    BOOL isSizeAdjusted;
}

- (id)initWithMIMEType:(NSString *)MIME;
- (id)initWithData:(NSData *)data MIMEType:(NSString *)MIME;
- (id)initWithContentsOfFile:(NSString *)filename;
- (NSString *)MIMEType;
+ (void)stopAnimationsInView:(NSView *)aView;
- (int)frameCount;
- (NSRect)targetAnimationRect;
- (void)resize:(NSSize)s;
- (NSSize)size;
- (NSData *)TIFFRepresentation;
- (NSImage *)image;

@end

#else   // Panther version of WebImageRenderer ------------------------------------

@class WebInternalImage;

@interface WebImageRenderer : NSObject <WebCoreImageRenderer>
{
    WebInternalImage *image;
}

- (id)initWithMIMEType:(NSString *)MIME;
- (id)initWithData:(NSData *)data MIMEType:(NSString *)MIME;
- (id)initWithContentsOfFile:(NSString *)filename;

- (NSImage *)image;
- (NSString *)MIMEType;
- (NSData *)TIFFRepresentation;
- (int)frameCount;

- (void)setOriginalData:(NSData *)data;

+ (void)stopAnimationsInView:(NSView *)aView;

@end

#endif

CGColorSpaceRef WebCGColorSpaceCreateRGB(void);
CGColorSpaceRef WebCGColorSpaceCreateGray(void);
CGColorSpaceRef WebCGColorSpaceCreateCMYK(void);
