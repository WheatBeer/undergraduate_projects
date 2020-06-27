package multithread_web;

import java.net.*;

public final class WebServer {
	private static ServerSocket ss;

	public static void main(String argv[]) throws Exception {
		// set the port number
		int port = 4444;
		ss = new ServerSocket(port);

		while (true) {
			// listening clients request
			Socket sc = ss.accept();
			// Construct an object to process the HTTP request message.
			HttpRequest request = new HttpRequest(sc);
			// Create a new thread to process the request.
			Thread thread = new Thread(request);
			// Start the thread.
			thread.start();
		}

	}
}
