#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    std::string type = "";
    AudioTrack* track = nullptr;
    int count = 0;
    for(int i = 0; i < library_tracks.size(); i++){
        type = library_tracks[i].type;
        if(type == "MP3"){

            track = new MP3Track(library_tracks[i].title, library_tracks[i].artists,
                library_tracks[i].duration_seconds, library_tracks[i].bpm,
                library_tracks[i].extra_param1, library_tracks[i].extra_param2);
                library.push_back(track);
                std::cout << "MP3Track created: " << library_tracks[i].extra_param1 <<" kbps\n";
                count++;
        }
        else{
            track = new WAVTrack(library_tracks[i].title, library_tracks[i].artists,
                library_tracks[i].duration_seconds, library_tracks[i].bpm,
                library_tracks[i].extra_param1, library_tracks[i].extra_param2);
                library.push_back(track);
                std::cout << " WAVTrack created: " << library_tracks[i].extra_param1 <<" Hz/" << library_tracks[i].extra_param2 << "bit\n";
                count++;
        }
    }
    std::cout <<  "Log summary: [INFO] Track library built:" << count << "tracks loaded";
;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << " [INFO] Loading playlist: " << playlist_name << "\n";
    Playlist p(playlist_name);
    for(int i = 0; i <track_indices.size(); i++){
        if(track_indices[i] >= library.size() || track_indices[i] < 0){
            std::cout << " [WARNING] Invalid track index: " << i <<"\n";
            continue;
        }
    
        PointerWrapper<AudioTrack> cloneWrap = library[track_indices[i]-1]->clone();
        if(!cloneWrap) {
            std::cout << " [WARNING] faild to log track at index\n";
            continue;
        }

        AudioTrack* cur = cloneWrap.release();
        cur->load();
        cur->analyze_beatgrid();
        playlist.add_track(cur);
        std::cout << "Added '" <<cur->get_title() << "' to playlist '" << playlist.get_name() << "'\n";

    }
    std::cout << "[INFO] Playlist loaded: " << playlist.get_name() << " " << playlist.get_track_count() << " tracks";
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {

    std::vector<std::string> titles;
    for(AudioTrack* t: playlist.getTracks())
    {
        titles.push_back(t->get_title());
    }
    return titles;
}

