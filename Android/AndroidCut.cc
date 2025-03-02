#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <regex>

namespace fs = std::filesystem;

void processLogFile(const std::string &logFilePath)
{
    std::ifstream logFile(logFilePath);
    if (!logFile.is_open())
    {
        std::cerr << "无法打开日志文件: " << logFilePath << std::endl;
        return;
    }

    std::string line;
    std::regex datePattern(R"(\d{2}-\d{2})"); // 匹配MM-DD格式
    while (std::getline(logFile, line))
    {
        std::istringstream iss(line);
        std::string date, temp, content;
        for (int i = 0; i < 6; ++i)
        {
            if (i == 0)
            {
                iss >> date;
            }
            else if (i == 5)
            {
                iss >> content;
            }
            else
            {
                iss >> temp;
            }
        }

        // 检查date是否符合MM-DD格式
        if (date.length() != 5 || !std::regex_match(date, datePattern))
        {
            continue; // 跳过该行
        }

        // 去掉content末尾的冒号
        if (!content.empty() && content.back() == ':')
        {
            content.pop_back();
        }

        // 只保留content的最后文件名部分
        fs::path contentPath(content);
        content = contentPath.filename().string();

        // 创建目录
        fs::create_directories("./" + date);

        // 输出到对应文件
        std::ofstream outFile("./" + date + "/" + content + ".txt", std::ios::app);
        if (!outFile.is_open())
        {
            std::cerr << "无法打开输出文件: " << "./" + date + "/" + content + ".txt" << std::endl;
            continue;
        }
        outFile << line << std::endl;
    }

    logFile.close();
}

void findAndProcessLogFiles(const std::string &rootPath)
{
    for (const auto &entry : fs::recursive_directory_iterator(rootPath))
    {
        if (entry.is_regular_file() && entry.path().filename() == "applogcat.log")
        {
            processLogFile(entry.path().string());
        }
    }
}

int main()
{
    std::string rootPath = "../"; // 替换为实际的根目录路径
    findAndProcessLogFiles(rootPath);
    return 0;
}