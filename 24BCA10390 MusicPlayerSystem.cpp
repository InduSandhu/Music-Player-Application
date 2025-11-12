#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

class Song {
public:
    string title;
    string artist;
    int durationSeconds;

    Song(string t, string a, int d) {
        title = t;
        artist = a;
        durationSeconds = d;
    }

    string formattedDuration() const {
        int m = durationSeconds / 60;
        int s = durationSeconds % 60;
        ostringstream out;
        out << setw(2) << setfill('0') << m << ":" 
            << setw(2) << setfill('0') << s;
        return out.str();
    }
};

class MusicPlayer {
private:
    vector<Song> playlist;

    // Load playlist from file
    void loadFromFile() {
        ifstream file("playlist.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            size_t tPos = line.find("Title: ");
            size_t aPos = line.find(" | Artist: ");
            size_t dPos = line.find(" | Duration: ");

            if (tPos == string::npos || aPos == string::npos || dPos == string::npos)
                continue;

            string title = line.substr(tPos + 7, aPos - (tPos + 7));
            string artist = line.substr(aPos + 10, dPos - (aPos + 10));
            string durationStr = line.substr(dPos + 12);

            int m, s;
            char colon;
            stringstream ss(durationStr);
            ss >> m >> colon >> s;

            playlist.push_back(Song(title, artist, m * 60 + s));
        }
        file.close();
    }

    // Save playlist to file
    void saveToFile() {
        ofstream file("playlist.txt");
        for (auto &song : playlist) {
            file << "Title: " << song.title
                 << " | Artist: " << song.artist
                 << " | Duration: " << song.formattedDuration() << endl;
        }
        file.close();
    }

public:
    MusicPlayer() {
        loadFromFile();
    }

    void addSong() {
        cin.ignore();
        string title, artist, durationStr;

        cout << "\nEnter Song Title: ";
        getline(cin, title);

        cout << "Enter Artist Name: ";
        getline(cin, artist);

        cout << "Enter Duration (MM:SS): ";
        getline(cin, durationStr);

        int m, s;
        char colon;
        stringstream ss(durationStr);
        ss >> m >> colon >> s;

        playlist.push_back(Song(title, artist, m * 60 + s));
        saveToFile();

        cout << "\nSong Added Successfully!\n";
    }

    void showPlaylist() {
        if (playlist.empty()) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        cout << "\nYour Playlist:\n";
        cout << left << setw(4) << "No."
             << setw(25) << "Title"
             << setw(25) << "Artist"
             << "Duration\n";

        cout << "-------------------------------------------------------------\n";

        for (int i = 0; i < playlist.size(); i++) {
            cout << setw(4) << i + 1
                 << setw(25) << playlist[i].title
                 << setw(25) << playlist[i].artist
                 << playlist[i].formattedDuration() << endl;
        }
    }

    void playSong() {
        if (playlist.empty()) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        showPlaylist();
        int choice;
        cout << "\nEnter song number to play: ";
        cin >> choice;

        if (choice > 0 && choice <= playlist.size()) {
            cout << "\nNow Playing: " 
                 << playlist[choice - 1].title 
                 << " by " << playlist[choice - 1].artist << endl;
        } else {
            cout << "\nInvalid selection.\n";
        }
    }

    void deleteSong() {
        if (playlist.empty()) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        showPlaylist();
        int choice;
        cout << "\nEnter song number to delete: ";
        cin >> choice;

        if (choice > 0 && choice <= playlist.size()) {
            playlist.erase(playlist.begin() + (choice - 1));
            saveToFile();
            cout << "\nSong Deleted Successfully!\n";
        } else {
            cout << "\nInvalid selection.\n";
        }
    }
};

int main() {
    MusicPlayer player;
    int option;

    while (true) {
        cout << "\n===== MUSIC PLAYER MENU =====\n";
        cout << "1. Add Song\n";
        cout << "2. Show Playlist\n";
        cout << "3. Play Song\n";
        cout << "4. Delete Song\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) {
            case 1: player.addSong(); break;
            case 2: player.showPlaylist(); break;
            case 3: player.playSong(); break;
            case 4: player.deleteSong(); break;
            case 5: return 0;
            default: cout << "\nInvalid Option! Try Again.\n";
        }
    }
}