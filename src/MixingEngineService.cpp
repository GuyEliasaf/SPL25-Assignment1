#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0), decks{nullptr, nullptr}
{
    
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
   for(int i = 0; i < 2; i++){
    delete decks[i];
    decks[i] = nullptr;
   }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> cloneWrap = track.clone();

    if (!cloneWrap) {
        std::cout << "[ERROR] Track: \"" << track.get_title()
                  << "\" failed to clone" << std::endl;
        return -1;                      
    }

    size_t target_deck = 1 - active_deck;

    if(decks[0] == nullptr && decks[1] == nullptr){
        target_deck = 0;
        active_deck = 0;
        cloneWrap.get()->load();
        cloneWrap.get()->analyze_beatgrid();
        decks[target_deck] = cloneWrap.release();
        std::cout << " [Load Complete] " << decks[target_deck]->get_title() <<" is now loaded on deck" << target_deck;
        return target_deck;
    }

    std::cout << "[Deck Switch] Target deck:" << target_deck;
    
    if(decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    cloneWrap.get()->load();
    cloneWrap.get()->analyze_beatgrid();

    

    if(decks[active_deck] && auto_sync){
        if(can_mix_tracks(cloneWrap)){
            sync_bpm(cloneWrap);
        }
    }
    decks[target_deck] = cloneWrap.release();
    std::cout << " [Load Complete] " << decks[target_deck]->get_title() <<" is now loaded on deck" << target_deck;

        if(decks[active_deck])
        {
            std::cout << "[Unload] Unloading previous deck" << active_deck << decks[active_deck]->get_title();
            delete decks[active_deck];
            decks[active_deck] = nullptr;
        }
    
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck" << target_deck;

    return target_deck; 
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] == nullptr) return false;
    if(track.get() == nullptr) return false;

    int dif = std::abs(track.get()->get_bpm() - decks[active_deck]->get_bpm());
     if(dif <= bpm_tolerance) {
        return true;
     }
    return false; 
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] && track.get()){
        int original_bpm = track.get()->get_bpm();
        track.get()->set_bpm((original_bpm + decks[active_deck]->get_bpm())/2);
    }
}
