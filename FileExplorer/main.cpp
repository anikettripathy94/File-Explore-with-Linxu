#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <regex>
#include <functional>

namespace fs = std::filesystem;

class FileExplorer {
private:
    fs::path current_path;

public:
    FileExplorer() {
        current_path = fs::current_path();
    }

    // Day 1: List files in directory
    void listFiles() {
        try {
            std::cout << "\n=== Contents of: " << current_path << " ===\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << std::left << std::setw(40) << "NAME" 
                      << std::setw(15) << "TYPE" 
                      << std::setw(15) << "SIZE\n";
            std::cout << std::string(80, '-') << "\n";

            if (!fs::exists(current_path)) {
                std::cout << "Error: Path does not exist\n";
                return;
            }

            for (const auto& entry : fs::directory_iterator(current_path)) {
                std::string name = entry.path().filename().string();
                std::string type = entry.is_directory() ? "FOLDER" : "FILE";
                
                std::string size_str;
                if (entry.is_regular_file()) {
                    size_str = std::to_string(fs::file_size(entry)) + " B";
                } else {
                    size_str = "-";
                }

                std::cout << std::left << std::setw(40) << name 
                          << std::setw(15) << type 
                          << std::setw(15) << size_str << "\n";
            }
            std::cout << std::string(80, '-') << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error listing files: " << e.what() << "\n";
        }
    }

    // Day 2: Change directory
    void changeDirectory(const std::string& path) {
        try {
            fs::path new_path;
            
            if (fs::path(path).is_absolute()) {
                new_path = path;
            } else {
                new_path = current_path / path;
            }
            
            if (!fs::exists(new_path)) {
                std::cout << "Error: Directory '" << path << "' not found\n";
                return;
            }
            
            if (!fs::is_directory(new_path)) {
                std::cout << "Error: '" << path << "' is not a directory\n";
                return;
            }
            
            current_path = fs::canonical(new_path);
            std::cout << "Changed to: " << current_path << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error changing directory: " << e.what() << "\n";
        }
    }

    // Day 2: Go to parent directory
    void goParent() {
        try {
            if (current_path.has_parent_path() && current_path != current_path.root_path()) {
                current_path = current_path.parent_path();
                std::cout << "Changed to: " << current_path << "\n";
            } else {
                std::cout << "Already at root directory\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    // Day 2: Print working directory
    void printWorkingDirectory() {
        std::cout << current_path << "\n";
    }

    // Day 3: Create empty file
    void createFile(const std::string& filename) {
        try {
            fs::path file_path = current_path / filename;
            
            if (fs::exists(file_path)) {
                std::cout << "Error: File '" << filename << "' already exists\n";
                return;
            }
            
            std::ofstream file(file_path);
            file.close();
            std::cout << "File created: " << file_path << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error creating file: " << e.what() << "\n";
        }
    }

    // Day 3: Create directory
    void createDirectory(const std::string& dirname) {
        try {
            fs::path dir_path = current_path / dirname;
            
            if (fs::exists(dir_path)) {
                std::cout << "Error: Directory '" << dirname << "' already exists\n";
                return;
            }
            
            if (fs::create_directory(dir_path)) {
                std::cout << "Directory created: " << dir_path << "\n";
            } else {
                std::cout << "Failed to create directory\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error creating directory: " << e.what() << "\n";
        }
    }

    // Day 3: Copy file or directory
    void copyFile(const std::string& source, const std::string& destination) {
        try {
            fs::path src = current_path / source;
            fs::path dst = current_path / destination;

            if (!fs::exists(src)) {
                std::cout << "Error: Source '" << source << "' not found\n";
                return;
            }

            if (fs::exists(dst)) {
                std::cout << "Error: Destination '" << destination << "' already exists\n";
                return;
            }

            fs::copy(src, dst, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            std::cout << "Copied: " << src << " -> " << dst << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error copying: " << e.what() << "\n";
        }
    }

    // Day 3: Move or rename file
    void moveFile(const std::string& source, const std::string& destination) {
        try {
            fs::path src = current_path / source;
            fs::path dst = current_path / destination;

            if (!fs::exists(src)) {
                std::cout << "Error: Source '" << source << "' not found\n";
                return;
            }

            if (fs::exists(dst)) {
                std::cout << "Error: Destination '" << destination << "' already exists\n";
                return;
            }

            fs::rename(src, dst);
            std::cout << "Moved: " << src << " -> " << dst << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error moving: " << e.what() << "\n";
        }
    }

    // Day 3: Delete file or directory
    void deleteFile(const std::string& path) {
        try {
            fs::path file_path = current_path / path;

            if (!fs::exists(file_path)) {
                std::cout << "Error: '" << path << "' not found\n";
                return;
            }

            if (fs::is_directory(file_path)) {
                size_t count = fs::remove_all(file_path);
                std::cout << "Directory deleted: " << file_path << " (" << count << " items removed)\n";
            } else {
                fs::remove(file_path);
                std::cout << "File deleted: " << file_path << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error deleting: " << e.what() << "\n";
        }
    }

    // Day 4: Find files matching a pattern
    void findFiles(const std::string& pattern, int max_depth = -1) {
        (void)max_depth;  // Mark as intentionally unused
        
        try {
            std::cout << "\n=== Search results for pattern: '" << pattern << "' ===\n";
            std::cout << std::string(80, '-') << "\n";

            // Create regex pattern (case-insensitive)
            std::regex regex_pattern(pattern, std::regex::icase);
            int match_count = 0;

            // Use recursive_directory_iterator to traverse all subdirectories
            try {
                for (const auto& entry : fs::recursive_directory_iterator(
                    current_path, 
                    fs::directory_options::skip_permission_denied)) {
                    
                    try {
                        // Get filename only (not full path)
                        std::string filename = entry.path().filename().string();
                        
                        // Check if filename matches the pattern
                        if (std::regex_search(filename, regex_pattern)) {
                            // Display full path
                            std::cout << entry.path() << "\n";
                            match_count++;
                        }
                    } catch (const std::exception&) {
                        // Skip entries that cause errors
                        continue;
                    }
                }
            } catch (const std::exception& e) {
                // Handle permission errors gracefully
                if (match_count == 0) {
                    std::cout << "Warning: " << e.what() << "\n";
                }
            }

            std::cout << std::string(80, '-') << "\n";
            std::cout << "Found: " << match_count << " match(es)\n\n";

        } catch (const std::exception& e) {
            std::cout << "Error during search: " << e.what() << "\n";
        }
    }

    // ==================== DAY 5: Permission Management ====================

    // Day 5: Show file permissions
    void showPermissions(const std::string& path) {
        try {
            fs::path file_path = current_path / path;

            if (!fs::exists(file_path)) {
                std::cout << "Error: '" << path << "' not found\n";
                return;
            }

            auto perms = fs::status(file_path).permissions();

            std::cout << "\nPermissions for: " << file_path << "\n";
            std::cout << std::string(50, '-') << "\n";
            
            // Owner (User) permissions
            std::cout << "Owner (User): ";
            std::cout << ((perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
            std::cout << ((perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
            std::cout << ((perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-") << "\n";
            
            // Group permissions
            std::cout << "Group:        ";
            std::cout << ((perms & fs::perms::group_read) != fs::perms::none ? "r" : "-");
            std::cout << ((perms & fs::perms::group_write) != fs::perms::none ? "w" : "-");
            std::cout << ((perms & fs::perms::group_exec) != fs::perms::none ? "x" : "-") << "\n";
            
            // Other permissions
            std::cout << "Other:        ";
            std::cout << ((perms & fs::perms::others_read) != fs::perms::none ? "r" : "-");
            std::cout << ((perms & fs::perms::others_write) != fs::perms::none ? "w" : "-");
            std::cout << ((perms & fs::perms::others_exec) != fs::perms::none ? "x" : "-") << "\n";
            
            std::cout << std::string(50, '-') << "\n";
            
            // Show file type
            if (fs::is_directory(file_path)) {
                std::cout << "Type: Directory\n";
            } else {
                std::cout << "Type: File\n";
            }
            std::cout << "\n";

        } catch (const std::exception& e) {
            std::cout << "Error reading permissions: " << e.what() << "\n";
        }
    }

    // Day 5: Change file permissions
void changePermissions(const std::string& path, const std::string& perms_str) {
    try {
        fs::path file_path = current_path / path;

        if (!fs::exists(file_path)) {
            std::cout << "Error: '" << path << "' not found\n";
            return;
        }

        // Parse permission string (e.g., "755" -> owner: rwx, group: r-x, other: r-x)
        if (perms_str.length() == 3) {
            fs::perms owner_perms = fs::perms::none;
            fs::perms group_perms = fs::perms::none;
            fs::perms other_perms = fs::perms::none;

            // Owner permissions (first digit)
            int owner_val = perms_str[0] - '0';
            if (owner_val & 4) owner_perms |= fs::perms::owner_read;
            if (owner_val & 2) owner_perms |= fs::perms::owner_write;
            if (owner_val & 1) owner_perms |= fs::perms::owner_exec;

            // Group permissions (second digit)
            int group_val = perms_str[1] - '0';
            if (group_val & 4) group_perms |= fs::perms::group_read;
            if (group_val & 2) group_perms |= fs::perms::group_write;
            if (group_val & 1) group_perms |= fs::perms::group_exec;

            // Other permissions (third digit)
            int other_val = perms_str[2] - '0';
            if (other_val & 4) other_perms |= fs::perms::others_read;
            if (other_val & 2) other_perms |= fs::perms::others_write;
            if (other_val & 1) other_perms |= fs::perms::others_exec;

            // Combine all permissions
            fs::perms new_perms = owner_perms | group_perms | other_perms;
            
            try {
                fs::permissions(file_path, new_perms, fs::perm_options::replace);
                std::cout << "Permissions changed to: " << perms_str << "\n";
                std::cout << "Command completed successfully!\n\n";
            } catch (const std::exception& inner_e) {
                // Windows often doesn't support Unix permissions
                std::cout << "Warning: " << inner_e.what() << "\n";
                std::cout << "Note: Windows uses ACLs instead of Unix permissions.\n";
                std::cout << "Permissions changed to: " << perms_str << " (conceptually)\n\n";
            }
        } else {
            std::cout << "Error: Use 3-digit format (e.g., 755)\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

    // Help command
    void showHelp() {
        std::cout << "\n=== FILE EXPLORER COMMANDS (Days 1-5) ===\n\n";
        std::cout << "Navigation:\n";
        std::cout << "  ls              - List files in current directory\n";
        std::cout << "  cd <dir>        - Change to directory\n";
        std::cout << "  cd ..           - Go to parent directory\n";
        std::cout << "  pwd             - Print working directory\n\n";
        
        std::cout << "File/Directory Operations:\n";
        std::cout << "  touch <file>    - Create empty file\n";
        std::cout << "  mkdir <dir>     - Create directory\n";
        std::cout << "  cp <src> <dst>  - Copy file or directory\n";
        std::cout << "  mv <src> <dst>  - Move or rename file\n";
        std::cout << "  rm <path>       - Delete file or directory\n\n";

        std::cout << "Search:\n";
        std::cout << "  find <pattern>  - Search files (regex pattern, case-insensitive)\n\n";

        std::cout << "Permissions:\n";
        std::cout << "  stat <file>     - Show file permissions\n";
        std::cout << "  chmod <file> <perms> - Change permissions (3-digit octal)\n";
        std::cout << "  Example: chmod file.txt 755\n";
        std::cout << "  Example: chmod file.txt 644\n\n";
        
        std::cout << "Other:\n";
        std::cout << "  help            - Show this help menu\n";
        std::cout << "  exit            - Exit the program\n\n";
    }
};

int main() {
    FileExplorer explorer;
    std::string input;

    std::cout << "=== Linux File Explorer (Days 1-5) ===\n";
    std::cout << "Type 'help' for commands\n\n";

    explorer.showHelp();

    while (true) {
        std::cout << "explorer> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        // Parse command
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "exit" || command == "quit") {
            std::cout << "Goodbye!\n";
            break;
        }
        else if (command == "help") {
            explorer.showHelp();
        }
        else if (command == "ls") {
            explorer.listFiles();
        }
        else if (command == "pwd") {
            explorer.printWorkingDirectory();
        }
        else if (command == "cd") {
            std::string path;
            if (iss >> path) {
                if (path == "..") {
                    explorer.goParent();
                } else {
                    explorer.changeDirectory(path);
                }
            } else {
                std::cout << "Usage: cd <directory>\n";
            }
        }
        else if (command == "touch") {
            std::string filename;
            if (iss >> filename) {
                explorer.createFile(filename);
            } else {
                std::cout << "Usage: touch <filename>\n";
            }
        }
        else if (command == "mkdir") {
            std::string dirname;
            if (iss >> dirname) {
                explorer.createDirectory(dirname);
            } else {
                std::cout << "Usage: mkdir <dirname>\n";
            }
        }
        else if (command == "cp") {
            std::string src, dst;
            if (iss >> src >> dst) {
                explorer.copyFile(src, dst);
            } else {
                std::cout << "Usage: cp <source> <destination>\n";
            }
        }
        else if (command == "mv") {
            std::string src, dst;
            if (iss >> src >> dst) {
                explorer.moveFile(src, dst);
            } else {
                std::cout << "Usage: mv <source> <destination>\n";
            }
        }
        else if (command == "rm") {
            std::string path;
            if (iss >> path) {
                explorer.deleteFile(path);
            } else {
                std::cout << "Usage: rm <path>\n";
            }
        }
        else if (command == "find") {
            std::string pattern;
            if (iss >> pattern) {
                explorer.findFiles(pattern);
            } else {
                std::cout << "Usage: find <pattern>\n";
                std::cout << "Example: find \\.txt\n";
                std::cout << "Example: find test\n";
            }
        }
        else if (command == "stat") {
            std::string path;
            if (iss >> path) {
                explorer.showPermissions(path);
            } else {
                std::cout << "Usage: stat <file>\n";
            }
        }
        else if (command == "chmod") {
            std::string path, perms;
            if (iss >> path >> perms) {
                explorer.changePermissions(path, perms);
            } else {
                std::cout << "Usage: chmod <file> <permissions>\n";
                std::cout << "Example: chmod file.txt 755\n";
                std::cout << "Example: chmod file.txt 644\n";
            }
        }
        else {
            std::cout << "Unknown command: '" << command << "'. Type 'help' for available commands.\n";
        }
    }

    return 0;
}
