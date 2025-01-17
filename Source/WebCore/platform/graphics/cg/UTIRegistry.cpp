/*
 * Copyright (C) 2017-2018 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "UTIRegistry.h"

#if USE(CG)

#include "ImageSourceCG.h"
#include "MIMETypeRegistry.h"

#include <wtf/HashSet.h>
#include <wtf/NeverDestroyed.h>
#include <ImageIO/ImageIO.h>

namespace WebCore {

const HashSet<String>& defaultSupportedImageTypes()
{
    static const auto defaultSupportedImageTypes = makeNeverDestroyed([] {
        const String defaultSupportedImageTypes[] = {
            "com.compuserve.gif"_s,
            "com.microsoft.bmp"_s,
            "com.microsoft.cur"_s,
            "com.microsoft.ico"_s,
            "public.jpeg"_s,
            "public.png"_s,
            "public.tiff"_s,
#if !PLATFORM(WIN)
            "public.jpeg-2000"_s,
            "public.mpo-image"_s,
#endif
#if HAVE(WEBP)
            "public.webp"_s,
            "com.google.webp"_s,
#endif
        };

        RetainPtr<CFArrayRef> systemImageTypes = adoptCF(CGImageSourceCopyTypeIdentifiers());
        CFIndex count = CFArrayGetCount(systemImageTypes.get());

        HashSet<String> defaultCGSupportedImageTypes;
        for (auto& imageType : defaultSupportedImageTypes) {
            RetainPtr<CFStringRef> string = imageType.createCFString();
            if (CFArrayContainsValue(systemImageTypes.get(), CFRangeMake(0, count), string.get()))
                defaultCGSupportedImageTypes.add(imageType);
        }
        return defaultCGSupportedImageTypes;
    }());

    return defaultSupportedImageTypes;
}

HashSet<String>& additionalSupportedImageTypes()
{
    static NeverDestroyed<HashSet<String>> additionalSupportedImageTypes;
    return additionalSupportedImageTypes;
}

void setAdditionalSupportedImageTypes(const Vector<String>& imageTypes)
{
    MIMETypeRegistry::additionalSupportedImageMIMETypes().clear();
    for (const auto& imageType : imageTypes) {
        additionalSupportedImageTypes().add(imageType);
        auto mimeType = MIMETypeForImageType(imageType);
        if (!mimeType.isEmpty())
            MIMETypeRegistry::additionalSupportedImageMIMETypes().add(mimeType);
    }
}

void setAdditionalSupportedImageTypesForTesting(const String& imageTypes)
{
    setAdditionalSupportedImageTypes(imageTypes.split(';'));
}

bool isSupportedImageType(const String& imageType)
{
    if (imageType.isEmpty())
        return false;
    return defaultSupportedImageTypes().contains(imageType) || additionalSupportedImageTypes().contains(imageType);
}

bool isGIFImageType(StringView imageType)
{
    return imageType == "com.compuserve.gif";
}

}

#endif
