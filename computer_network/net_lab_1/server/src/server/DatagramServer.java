package server;

import java.io.*;
import java.net.*;

public class DatagramServer {
	public static void main(String[] args) throws Exception {

		DatagramSocket ds = new DatagramSocket(5555);
		byte buf[] = new byte[100];
		DatagramPacket dp = new DatagramPacket(buf, buf.length);
		ds.receive(dp);
		String tmp = new String(dp.getData());
		System.out.print("수신 : " + tmp);
	}

}