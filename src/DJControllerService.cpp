#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {

    if(cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }

    PointerWrapper<AudioTrack> cloneWrap = track.clone();
    AudioTrack* cur = cloneWrap.release();

    if(!cur){
        std::cout << "Track cant be nullptr";
        return -2;
    }
    
    
    cur->load();
    cur->analyze_beatgrid();
    bool check = cache.put(std::move(PointerWrapper<AudioTrack>(cur)));
    if(!check) return 0;
    return -1;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return nullptr; // Placeholder
}
