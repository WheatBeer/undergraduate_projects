package server;

import java.io.*;
import java.net.*;

public class DatagramEchoServer {
	public static void main(String[] args) throws Exception {

		DatagramSocket ds = new DatagramSocket(5555);
		int n = 0;
		while (true) {
			
			if (n == 0) {
				byte buf[] = new byte[100];
				DatagramPacket dp = new DatagramPacket(buf, buf.length);
				ds.receive(dp);
				InetAddress ia = dp.getAddress();
				System.out.println(n+1 + "번째 통신");
				System.out.println(ia + "와(과) 연결되었습니다.");
				int portnum = dp.getPort();
				System.out.println("clientportnumber:" + portnum);
				String tmp = new String(dp.getData());
				System.out.println("수신 : " + tmp);
				byte buf1[] = tmp.getBytes();
				DatagramPacket dp1 = new DatagramPacket(buf1, buf1.length, ia, portnum);
				ds.send(dp);
				System.out.println("송신 : " + tmp);
				n = n + 1;
			} else {
				byte buf[] = new byte[100];
				DatagramPacket dp = new DatagramPacket(buf, buf.length);
				ds.receive(dp);
				System.out.println(n+1 + "번째 통신");
				InetAddress ia = dp.getAddress();
				int portnum = dp.getPort();
				System.out.println("clientportnumber:" + portnum);
				String tmp = new String(dp.getData());
				System.out.println("수신 : " + tmp);
				byte buf1[] = tmp.getBytes();
				DatagramPacket dp1 = new DatagramPacket(buf1, buf1.length, ia, portnum);
				ds.send(dp);
				System.out.println("송신 : " + tmp);
				n = n + 1;
			}
		}

	}
}