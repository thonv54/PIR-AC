// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"

// Custom event stubs. Custom events will be run along with all other events in
// the application framework. They should be managed using the Ember Event API
// documented in stack/include/events.h


///** @brief Main Init
// *
// * This function is called from the application's main function. It gives the
// * application a chance to do any initialization required at system startup. Any
// * code that you would normally put into the top of the application's main()
// * routine should be put into this function. This is called before the clusters,
// * plugins, and the network are initialized so some functionality is not yet
// * available.
//        Note: No callback in the Application Framework is
// * associated with resource cleanup. If you are implementing your application on
// * a Unix host where resource cleanup is a consideration, we expect that you
// * will use the standard Posix system calls, including the use of atexit() and
// * handlers for signals such as SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If
// * you use the signal() function to register your signal handler, please mind
// * the returned value which may be an Application Framework function. If the
// * return value is non-null, please make sure that you call the returned
// * function from your handler to avoid negating the resource cleanup of the
// * Application Framework itself.
// *
// */
//void emberAfMainInitCallback(void)
//{
//}





/** @brief Main Tick
 *
 * Whenever main application tick is called, this callback will be called at the
 * end of the main tick execution.
 *
 */
void emberAfMainTickCallback(void)
{
}


