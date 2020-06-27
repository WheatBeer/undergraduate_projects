package client;

import java.net.*;
import java.io.*;

public class DatagramClient {
	public static void main(String[] args) throws Exception {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		DatagramSocket ds = new DatagramSocket();
		InetAddress ia = InetAddress.getByName("127.0.0.1");
		System.out.print("송신: ");
		String tmp = "네트워크 실험입니다.";
		byte buf[] = tmp.getBytes();
		DatagramPacket dp = new DatagramPacket(buf, buf.length, ia, 5555);
		ds.send(dp);
		System.out.print(tmp);
	}
}