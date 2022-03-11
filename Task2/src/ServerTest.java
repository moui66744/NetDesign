import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerTest {
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(9999);
            while (true) {
                Socket socket = serverSocket.accept();
                PrintWriter sender = new PrintWriter(socket.getOutputStream());
                BufferedReader recver = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                while (true) {
                    String cmd = recver.readLine();
                    System.out.println("catch cmd: " + cmd);
                    String result = "this is output";
                    if (cmd.indexOf("exit") != -1) {
                        System.out.println("断开连接");
                        break;
                    }
                    else if (cmd.indexOf("in switch ") != -1) {//待定
                        result = cmd.substring(10);
                    }
                    sender.println(result);
                    sender.flush();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
