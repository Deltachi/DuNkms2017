//   ___     __ ___                                       _
//  |_ _|   / // _ \     ___ _   _ _ __  _ __   ___  _ __| |_
//   | |   / /| | | |   / __| | | | '_ \| '_ \ / _ \| '__| __|
//   | |  / / | |_| |   \__ \ |_| | |_) | |_) | (_) | |  | |_
//  |___|/_/   \___/    |___/\__,_| .__/| .__/ \___/|_|   \__|
//                                |_|   |_|

/**
 * Prepares support library for operation.
 *
 * Call this once at (or near) program start.
 */
void support_init();


/**
 * Clears the screen.
 */
void support_clear();


/**
 * Waits up to "timeout_ms" milliseconds for key press.
 *
 * Returns ASCII value or 0 (if no key was pressed).
 */
int support_readkey(int timeout_ms);

