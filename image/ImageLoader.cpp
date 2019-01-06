#include <IL/il.h>

#include "ImageLoader.h"
#include "RawImage.h"

namespace Core {

    Bool ImageLoader::initialized = false;

    Bool ImageLoader::initialize() {
        if (!ImageLoader::initialized) {
            if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
                std::cerr << "ImageLoader::initialize -> wrong DevIL version" << std::endl;
                // throw ImageLoaderException("ImageLoader::initialize -> wrong DevIL version");
            }

            ilInit(); /// Initialization of DevIL
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
            ilEnable(IL_ORIGIN_SET);
            ImageLoader::initialized = true;
        }

        return true;
    }

    std::shared_ptr<RawImage> ImageLoader::loadImageU(const std::string& fullPath) {
        return loadImageU(fullPath, false);
    }

    std::shared_ptr<RawImage> ImageLoader::loadImageU(const std::string& fullPath, Bool reverseOrigin) {
        Bool initializeSuccess = initialize();

        if (!initializeSuccess) {
            throw ImageLoaderException("ImageLoader::LoadImage -> Error occurred while initializing image loader.");
        }
     
        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
        }

        std::string extension = getFileExtension(fullPath);

        ILuint imageIds[1];
        ilGenImages(1, imageIds); // Generation of numTextures image names
        ilBindImage(imageIds[0]); // Binding of DevIL image name
        std::shared_ptr<RawImage> rawImage;

        ILboolean success = ilLoadImage(fullPath.c_str());

        if (success) {
            // Convert every color component into unsigned byte.If your image contains
            // alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            if (!success) {
                ilDeleteImages(1, imageIds);
                throw ImageLoaderException("ImageLoader::LoadImage -> Couldn't convert image");
            }
            rawImage = getRawImageFromILData(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
        } else {
            ILenum i = ilGetError();
            std::string msg = "ImageLoader::LoadImage -> Couldn't load image: ";
            msg += fullPath.c_str();
            if (i == IL_INVALID_EXTENSION) {
                msg = std::string("ImageLoader::LoadImage -> Couldn't load image (invalid extension). ");
                msg += std::string("Is DevIL configured to load extension: ") + extension + std::string(" ?");
            }
            ilDeleteImages(1, imageIds);
            throw ImageLoaderException(msg);
        }

        // Because we have already copied image data into texture data we can release memory used by image.
        ilDeleteImages(1, imageIds);

        if (reverseOrigin) {
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        }

        return rawImage;
    }

    std::shared_ptr<RawImage> ImageLoader::getRawImageFromILData(const ILubyte * data, UInt32 width, UInt32 height) {
        if (data == nullptr) throw ImageLoaderException("ImageLoader::getRawImageFromILData -> 'data' is null.");

        RawImage * rawImagePtr = new(std::nothrow) RawImage(width, height);
        if (rawImagePtr == nullptr) throw ImageLoaderException("ImageLoader::getRawImageFromILData -> Could not allocate RawImage.");

        std::shared_ptr<RawImage> rawImage(rawImagePtr);

        Bool initSuccess = rawImage->init();
        if (!initSuccess) {
            throw ImageLoaderException("ImageLoader::getRawImageFromILData -> Could not init RawImage.");
        }

        for (UInt32 i = 0; i < width * height * 4; i++) {
            rawImage->setByte(i, (Byte)data[i]);
        }

        return rawImage;
    }
 
    std::string ImageLoader::getFileExtension(const std::string& filePath) {
        Int32 dotIndex = (Int32)filePath.find_last_of(".");
        if (dotIndex < 0)dotIndex = 0;
        std::string extension = filePath.substr(dotIndex, filePath.size() - dotIndex);
        return extension;
    }

}