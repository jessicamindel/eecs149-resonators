
/* usage: (program) (sf2 file) (midi file) (tick to start at)
    basically play it until you want to, it listens at every tick for a stop signal, and then it stops.
    if you want to resume, main computer simply sends the tick to restart at (which is useful, since stops can be delayed due to latency)
*/

#include <fluidsynth.h>

int main(int argc, char** argv) 
{
    int i;
    fluid_settings_t* settings;
    fluid_synth_t* synth;
    fluid_player_t* player;
    fluid_audio_driver_t* adriver;
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    player = new_fluid_player(synth);
    /* process command line arguments */
    fluid_synth_sfload(synth, argv[0], 1);
    //add in the midi player
    fluid_player_add(player, argv[1]);
    //add in the tick seek
    fluid_player_seek(player, argv[2]);
    //start the interrupt handler (so basically when we need to stop it)
    fluid_player_set_tick_callback(player, interrupt_handler, player, settings, synth, adriver)
    /* start the synthesizer thread */
    adriver = new_fluid_audio_driver(settings, synth);
    /* play the midi files, if any */
    fluid_player_play(player);
    /* wait for playback termination */
    fluid_player_join(player);
    /* cleanup */
    delete_fluid_audio_driver(adriver);
    delete_fluid_player(player);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 0;
}

//this function runs every single MIDI tick so it basically insta stops once the stop code is recieved
void interrupt_handler(fluid_player_t* player, fluid_settings_t* settings, fluid_synth_t* synth, fluid_audio_driver_t* adriver) {
    //TODO: find a way to send a signal over BLE/Wifi, once that's done, just run the code block below lol
    if(false) {
        fluid_player_stop(player)
        delete_fluid_audio_driver(adriver);
        delete_fluid_player(player);
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
    }
}