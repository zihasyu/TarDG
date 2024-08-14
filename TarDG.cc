#include "TarDG.h"

std::string sha256(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void process_file(const std::filesystem::path &file_path, const std::string &output_dir)
{
    std::ifstream input_file(file_path, std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    std::map<std::string, std::ofstream> file_map;
    std::vector<char> buffer(BUFFER_SIZE);
    std::streamsize bytes_read;

    while ((bytes_read = input_file.readsome(buffer.data(), buffer.size())) > 0)
    {
        std::istringstream iss(std::string(buffer.data(), bytes_read));
        std::string line;

        while (std::getline(iss, line))
        {
            // 跳过空行或只有回车符的行
            if (line.empty() || line == "\r")
            {
                continue;
            }

            std::istringstream line_stream(line);
            std::string date, component, temp;

            // Skip to 3rd column
            for (int i = 0; i < 2; ++i)
                line_stream >> temp;
            line_stream >> date;

            // Skip to 9th column
            for (int i = 0; i < 5; ++i)
                line_stream >> temp;
            line_stream >> component;
            component = component.substr(0, component.find('[')); // Remove brackets

            // Check if date and component are valid
            if (date.empty() || component.empty())
            {
                std::cerr << "Invalid line: " << line << std::endl;
                continue;
            }
            if (date.length() != 10 || date[0] != '2')
            {
                std::cerr << "Invalid date: " << date << " in line: " << line << std::endl;
                continue;
            }

            // Hash the component to create a safe filename
            std::string hashed_component = sha256(component);

            // Ensure the directory exists
            std::filesystem::create_directories(output_dir + "/" + date);

            std::string filename = output_dir + "/" + date + "/" + hashed_component + ".log";

            if (file_map.find(filename) == file_map.end())
            {
                file_map[filename] = std::ofstream(filename, std::ios_base::app);
            }
            file_map[filename] << line << "\n";
        }

        // If remaining buffer is less than MIN_BUFFER_SIZE, seek back to adjust
        if (iss.rdbuf()->in_avail() < MIN_BUFFER_SIZE)
        {
            input_file.seekg(-iss.rdbuf()->in_avail(), std::ios_base::cur);
        }
    }
}
// void process_file(const std::filesystem::path& file_path, const std::string& output_dir) {
//     std::ifstream input_file(file_path, std::ios::binary);
//     if (!input_file) {
//         std::cerr << "Failed to open file: " << file_path << std::endl;
//         return;
//     }

//     std::map<std::string, std::ofstream> file_map;
//     std::vector<char> buffer(BUFFER_SIZE);
//     std::streamsize bytes_read;

//     while ((bytes_read = input_file.readsome(buffer.data(), buffer.size())) > 0) {
//         std::istringstream iss(std::string(buffer.data(), bytes_read));
//         std::string line;

//         while (std::getline(iss, line)) {
//             std::istringstream line_stream(line);
//             std::string date, component, temp;

//             // Skip to 3rd column
//             for (int i = 0; i < 2; ++i) line_stream >> temp;
//             line_stream >> date;

//             // Skip to 9th column
//             for (int i = 0; i < 5; ++i) line_stream >> temp;
//             line_stream >> component;
//             component = component.substr(0, component.find('[')); // Remove brackets

//             // Hash the component to create a safe filename
//             std::string hashed_component = sha256(component);

//             // Ensure the directory exists
//             std::filesystem::create_directories(output_dir + "/" + date);

//             std::string filename = output_dir + "/" + date + "/" + hashed_component + ".log";

//             if (file_map.find(filename) == file_map.end()) {
//                 file_map[filename] = std::ofstream(filename, std::ios_base::app);
//             }
//             file_map[filename] << line << "\n";
//         }

//         // If remaining buffer is less than MIN_BUFFER_SIZE, seek back to adjust
//         if (iss.rdbuf()->in_avail() < MIN_BUFFER_SIZE) {
//             input_file.seekg(-iss.rdbuf()->in_avail(), std::ios_base::cur);
//         }
//     }
// }

int main()
{
    std::string input_dir = "../DBset/split_files"; // 输入目录
    std::string output_dir = "../DBset/output";     // 输出目录

    std::vector<std::thread> threads;

    for (const auto &entry : std::filesystem::directory_iterator(input_dir))
    {
        if (entry.is_regular_file())
        {
            threads.emplace_back(process_file, entry.path(), output_dir);
        }
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}