#pragma once
#include <iostream>
#include <vector>
#include <filesystem>

struct CharacterSet
{
    std::wstring directory;
    std::vector<std::wstring> extraParts;
};

bool ensureDirectoryExists(std::filesystem::path directory)
{
    std::error_code error;
    bool created = std::filesystem::create_directories(directory, error);

    if (error)
    {
        std::wcout << L"[BLAD KATALOGU] " << directory << L": " << error.message().c_str() << '\n';
        return false;
    }

    if (created)
        std::wcout << L"[UTWORZONO KATALOG] " << directory << '\n';

    return true;
}

bool checkFile(const std::filesystem::path file)
{
    std::error_code error;
    auto status = std::filesystem::status(file, error);

    if (status.type() == std::filesystem::file_type::not_found)
    {
        std::wcout << L"[BRAK] " << file << '\n';
        return false;
    }

    if (error)
    {
        std::wcout << "L[BLAD] " << file << ": " << error.message().c_str() << '\n';
        return false;
    }

    if (!std::filesystem::is_regular_file(status))
    {
        std::wcout << L"[NIE JEST PLIKIEM] " << file << '\n';
        return false;
    }

    auto size = std::filesystem::file_size(file, error);
    if (error)
    {
        std::wcout << L"[BLAD] " << file << ": " << error.message().c_str() << '\n';
        return false;
    }

    if (size == 0)
    {
        std::wcout << L"[PUSTY] " << file << '\n';
        return false;
    }

    return true;
}

bool printLatestModifiedFile(std::filesystem::path root)
{
    std::optional<std::filesystem::path> lastModified;
    std::filesystem::file_time_type lastModifiedTime{};

    bool complete = true;
    auto reportError = [&](const std::filesystem::path& file, const std::error_code& error)
        {
            std::wcout << L"[BLAD SKANOWANIA] " << file << L": " << error.message().c_str() << '\n';
            complete = false;
        };

    std::error_code error;
    std::filesystem::recursive_directory_iterator it(root, error);
    std::filesystem::recursive_directory_iterator end;
    if (error)
        reportError(root, error);

    while (it != end)
    {
        std::filesystem::path file = it->path();
        bool regular = it->is_regular_file(error);

        if (error)
        {
            reportError(file, error);
        }
        else if (regular)
        {
            auto modified = std::filesystem::last_write_time(file, error);
            if (error)
            {
                reportError(file, error);
            }
            else if (!lastModified || modified > lastModifiedTime ||
                (modified == lastModifiedTime && file < *lastModified))
            {
                lastModified = file;
                lastModifiedTime = modified;
            }
        }

        it.increment(error);
        if (error)
        {
            reportError(root, error);
            break;
        }
    }

    std::wcout << '\n';
    if (!complete)
        std::wcout << L"Wynik niepelny: nie wszystkie pliki/daty udalo sie sprawdzic.\n";

    std::wcout << L"Ostatnio zmodyfikowany plik: ";
    if (lastModified)
        std::wcout << *lastModified << '\n';
    else
        std::wcout << L"brak dostepnego wyniku\n";

    return complete;
}

int checkCharactersSets()
{
    std::filesystem::path requestedRoot = "assets\\tex\\characters";

    std::error_code error;
    std::filesystem::path root = std::filesystem::absolute(requestedRoot, error);

    if (error)
    {
        std::wcout << L"Nie mozna ustalic sciezki: " << error.message().c_str() << '\n';
        return 2;
    }

    if (!ensureDirectoryExists(root))
        return 2;

    std::vector<std::wstring> animations = { L"idle", L"walk", L"attack"};
    std::wstring extension = L".png";

    std::vector<std::wstring> commonParts = {
        L"legs", 
        L"pants_01", L"pants_02", L"pants_03",
       
        L"torso", 
        L"armor_01", L"armor_02", L"armor_03",
       
        L"left_arm", 
        L"left_sleeve_01", L"left_sleeve_02", L"left_sleeve_03",
        
        L"right_arm", 
        L"right_sleeve_01", L"right_sleeve_02", L"right_sleeve_03",
        
        L"head_01", L"head_02", 
        L"helmet_01", L"helmet_02", L"helmet_03"
    };

    std::vector<CharacterSet> sets = {
        { L"empty", {} },
        { L"1h", { L"weapon", L"shield" } },
        { L"2h", { L"weapon" } },
        { L"spire", { L"weapon" } },
        { L"bow", { L"weapon" } },
        { L"magic_single", {} },
        { L"magic_area", {} },
        { L"magic_buff", {} }
    };

    for (auto& set : sets)
    {
        if (!ensureDirectoryExists(root / set.directory))
            return 2;
    }

    int checked = 0;
    int passed = 0;

    std::wcout << L"Sprawdzany katalog: " << root << L"\n\n";

    for (const auto& set : sets)
    {
        auto parts = commonParts;
        parts.insert(parts.end(), set.extraParts.begin(), set.extraParts.end());

        for (const auto& part : parts)
        {
            for (const auto& animation : animations)
            {
                const std::filesystem::path file = root / set.directory /
                    (part + L"_" + animation + extension);

                ++checked;
                if (checkFile(file))
                    ++passed;
            }
        }
    }

    std::wcout << L"\nOK: " << passed << '/' << checked << L" | Problemy: " << checked - passed << '\n';

    bool scanComplete = printLatestModifiedFile(root);
    return passed == checked && scanComplete ? 0 : 1;
}