#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <future>
#include <string>
#include <vector>

namespace GopherEngine
{
    // -------------------------------------------------------------------------
    // FileData
    //   Plain data bag returned when a load completes.  Intentionally simple.
    //   Upper layers are responsible for interpreting the bytes.
    // -------------------------------------------------------------------------
    struct FileData
    {
        std::filesystem::path path_;
        std::vector<std::uint8_t> bytes_;
        bool ok_{false};
        std::string error_;
    };

    class LoadHandle
    {
    public:

        using Callback = std::function<void(const FileData&)>;

        LoadHandle() = default;

        explicit LoadHandle(std::shared_future<FileData> future) : future_(std::move(future)) {}

        // Register a callback to be invoked on the main thread after the load completes.
        // Fluent API: FileLoader::load_file_async(...).on_complete([](auto& d){ ... });
        LoadHandle& on_complete(Callback cb) {
            callback_ = std::move(cb);
            return *this;
        }
        
        // Returns true if the background thread has finished (success OR error).
        // Safe to call every frame -- never blocks.
        bool is_ready() const {
            return future_.valid() && future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

        // Returns true if this handle refers to an in-flight or completed load.
        bool valid() const { 
            return future_.valid(); 
        }

        void fire_callback() {
            if(!callback_)
                return;

            callback_(future_.get());
        }

        // Call on the main thread once is_ready() returns true.
        // May be called multiple times (shared_future -- result is not consumed).
        FileData get() const { 
            return future_.get(); 
        }

        private:
            std::shared_future<FileData> future_;
            Callback callback_;
    };
    

    class FileLoader
    {
    public:

        static LoadHandle load_file_async(const std::filesystem::path& path);

        // Blocking call to read a file on the current thread.
        // Used internally by load_file_async.
        static FileData load_file(const std::filesystem::path& path);
    };
}