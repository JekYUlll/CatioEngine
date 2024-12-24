//
// Created by horeb on 24-11-28.
//
#ifndef CTFILEUTIL_H
#define CTFILEUTIL_H

#include "CtEngine.h"

#ifndef CT_DEFINE_RES_ROOT_DIR
#define CT_RES_ROOT_DIR CT_DEFINES_ROOT_DIR
#else
#define CT_RES_ROOT_DIR "Resource/"
#endif

#define CT_RES_CONFIG_DIR           CT_RES_ROOT_DIR"Config/"
#define CT_RES_SHADER_DIR           CT_RES_ROOT_DIR"Shader/"
#define CT_RES_FONT_DIR             CT_RES_ROOT_DIR"Font/"
#define CT_RES_MODEL_DIR            CT_RES_ROOT_DIR"Model/"
#define CT_RES_MATERIAL_DIR         CT_RES_ROOT_DIR"Material/"
#define CT_RES_TEXTURE_DIR          CT_RES_ROOT_DIR"Texture/"
#define CT_RES_SCENE_DIR            CT_RES_ROOT_DIR"Scene/"

namespace cte {

    static std::string GetFileName(const std::string &filePath){
        if(filePath.empty()){
            return filePath;
        }
        std::filesystem::path path(filePath);
        return path.filename().string();
    }

    static void FormatFileSize(std::uintmax_t fileSize, float *outSize, std::string &outUnit) {
        float size = static_cast<float>(fileSize);
        if (size < 1024) {
            outUnit = "B";
        } else if (size < 1024 * 1024) {
            size /= 1024;
            outUnit = "KB";
        } else if (size < 1024 * 1024 * 1024) {
            size /= (1024 * 1024);
            outUnit = "MB";
        } else {
            size /= (1024 * 1024 * 1024);
            outUnit = "GB";
        }
        *outSize = size;
    }

    static std::string FormatSystemTime(std::filesystem::file_time_type fileTime) {
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* tm = std::localtime(&time);

        std::stringstream ss;
        ss << std::put_time(tm, "%Y/%m/%d %H:%M");

        return ss.str();
    }

    static std::vector<char> ReadCharArrayFromFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if(!file.is_open()){
            throw std::runtime_error("Could not open the file: " + filePath);
        }

        auto fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

}

#endif //CTFILEUTIL_H
