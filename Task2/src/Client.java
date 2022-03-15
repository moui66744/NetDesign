/**
 * @author: why(moui), USTB
 * @time: 2022-03-15
 * @project: FPVGA
 * TELNET program Client based on TCP protocol
 * This project implements a simple FPGA emulator
 * The client does not need to install the Verilog runtime environment,
 * just upload the *.v file to the server
 * and then change the switch to display the Verilog execution result by LED
 * */

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.*;
import java.net.Socket;
import java.util.Arrays;
import java.util.Scanner;

import static java.lang.Thread.sleep;

public class Client implements Runnable {
    // Java GUI component
    public JFrame frame;
    public JPanel panel;
    public JPanel panel1;
    public JPanel panel2;
    public JPanel panel3;
    public JLabel[] LEDs;                           // 16 LEDs of fpvga
    public JToggleButton[] switches;                // 16 switches of fpvga
    public JLabel rst;                              // rst btn of fpvga
    public JButton chooseFileBtn;                   // choose file uploaded to Server
    public JToggleButton runBtn;                    // run fpvga
    public JButton openTerminalBtn;                 // open terminal
    public File filePath = new File(""); // Client file path of *.v file
    public int[] switchStatus = new int[16];        // record switch status
    // icon and font
    public ImageIcon LEDOnIcon;
    public ImageIcon LEDOffIcon;
    public ImageIcon switchOnIcon;
    public ImageIcon switchOffIcon;
    public ImageIcon rstOnIcon;
    public ImageIcon rstOffIcon;
    public ImageIcon runIcon;
    public ImageIcon terminalIcon;
    public ImageIcon uploadIcon;
    public Font font;
    //
    public Thread runThread;                        // another thread to run fpvga
    public boolean isRun;                           // runThread whether to continue
    public boolean enableRun;                       // to enable runBtn. If *.v file compilation error, enableRun = false
    public boolean isRst;                           // *.v file rst signal
    // socket
    public Socket socket;
    public PrintWriter sender;                      // send buffer
    public BufferedReader recver;                   // receive buffer

    Client() throws IOException {
        frame = new JFrame("FPVGA");        // title
        frame.setSize(1000, 400);  // window size 1000 * 400
        frame.setLocationRelativeTo(null);          // at the center of the screen
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        // set layout
        panel = new JPanel(new GridLayout(3, 1));
        panel.setBackground(Color.white);
        panel1 = new JPanel(new GridLayout(0, 16));
        panel1.setBackground(Color.white);
        panel2 = new JPanel(new GridLayout(0, 16));
        panel2.setBackground(Color.white);
        panel3 = new JPanel(new FlowLayout());
        panel3.setBackground(Color.white);
        // init icon and font
        LEDOnIcon = new ImageIcon("resources/LED_on.png");
        LEDOffIcon = new ImageIcon("resources/LED_off.png");
        switchOnIcon = new ImageIcon("resources/switch_on.png");
        switchOffIcon = new ImageIcon("resources/switch_off.png");
        rstOnIcon = new ImageIcon("resources/rst_on.png");
        rstOffIcon = new ImageIcon("resources/rst_off.png");
        runIcon = new ImageIcon("resources/run.png");
        terminalIcon = new ImageIcon("resources/terminal.png");
        uploadIcon = new ImageIcon("resources/terminal.png");
        font = new Font("console", Font.BOLD, 14);

        // create LED
        LEDs = new JLabel[16];
        for (int i=0; i<16; i++) {
            LEDs[i] = new JLabel();
            LEDs[i].setIcon(LEDOffIcon);
            panel1.add(LEDs[i]);
        }
        // create switch
        switches = new JToggleButton[16];
        for (int i=0; i<16; i++) {
            switches[i] = new JToggleButton();
            switches[i].setBorderPainted(false);
            switches[i].setFocusPainted(false);
            switches[i].setContentAreaFilled(false);
            switches[i].setBackground(Color.white);
            switches[i].setSelectedIcon(switchOnIcon);
            switches[i].setIcon(switchOffIcon);
            // add listener
            switches[i].addActionListener(e -> {
                JToggleButton toggleBtn1 = (JToggleButton) e.getSource();
                switchStatus[Arrays.asList(switches).indexOf(toggleBtn1)] = toggleBtn1.isSelected() ? 1 : 0;// change switch icon
            });
            panel2.add(switches[i]);
        }

        // add reset
        isRst = false;
        rst = new JLabel();
        rst.setIcon(rstOffIcon);
        // add mouse listener
        rst.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
            }
            @Override
            public void mousePressed(MouseEvent e) {// Rst enables when the mouse is pressed
                isRst = true;
                rst.setIcon(rstOnIcon);
            }
            @Override
            public void mouseReleased(MouseEvent e) {// Rst disables when the mouse is released
                isRst = false;
                rst.setIcon(rstOffIcon);
            }
            @Override
            public void mouseEntered(MouseEvent e) {
            }
            @Override
            public void mouseExited(MouseEvent e) {
            }
        });
        panel3.add(rst);

        // upload file button
        chooseFileBtn = new JButton("choose file");
        chooseFileBtn.setFont(font);
        chooseFileBtn.setPreferredSize(new Dimension(140, 100));
        chooseFileBtn.setBackground(new Color(0xf6f6f6));
        chooseFileBtn.setIcon(uploadIcon);
        chooseFileBtn.setHorizontalTextPosition(SwingConstants.CENTER);
        chooseFileBtn.setVerticalTextPosition(SwingConstants.BOTTOM);
        // listener
        chooseFileBtn.addActionListener(e -> {
            JFileChooser fileChooser = new JFileChooser();
            fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            // single file
            fileChooser.setMultiSelectionEnabled(false);
            // file filter: *.v
            fileChooser.setFileFilter(new FileNameExtensionFilter("verilog(*.v)", "v"));
            int result = fileChooser.showOpenDialog(frame);
            if (result == JFileChooser.APPROVE_OPTION) {
                // get file path
                filePath = fileChooser.getSelectedFile();
                uploadFile();
            }
        });
        panel3.add(chooseFileBtn);

        // run button
        runBtn = new JToggleButton("run");
        runBtn.setFont(font);
        runBtn.setPreferredSize(new Dimension(140, 100));
        runBtn.setBackground(new Color(0xf6f6f6));
        runBtn.setIcon(runIcon);
        runBtn.setHorizontalTextPosition(SwingConstants.CENTER);
        runBtn.setVerticalTextPosition(SwingConstants.BOTTOM);
        // listener
        runBtn.addActionListener(e -> {
            JToggleButton runBtn = (JToggleButton) e.getSource();
            if (runBtn.isSelected()) {
                runBtn.setText("stop");
                isRun = true;
                runThread = new Thread(this);
                runThread.start();
            } else {
                runBtn.setText("run");
                isRun = false;
            }
        });
        panel3.add(runBtn);

        // open terminal button
        openTerminalBtn = new JButton("open terminal");
        openTerminalBtn.setFont(font);
        openTerminalBtn.setPreferredSize(new Dimension(140, 100));
        openTerminalBtn.setBackground(new Color(0xf6f6f6));
        openTerminalBtn.setIcon(terminalIcon);
        openTerminalBtn.setHorizontalTextPosition(SwingConstants.CENTER);
        openTerminalBtn.setVerticalTextPosition(SwingConstants.BOTTOM);
        openTerminalBtn.addActionListener(e -> openTerminal());
        panel3.add(openTerminalBtn);

        panel.add(panel1);
        panel.add(panel2);
        panel.add(panel3);

        frame.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent e) {
            }
            @Override
            public void windowClosing(WindowEvent e) {// When the window closes, the command "shutdown" is sent to Server
                try {
                    sender.print("shutdown");
                    sender.flush();
                    // close connection
                    sender.close();
                    recver.close();
                    socket.close();
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
            @Override
            public void windowClosed(WindowEvent e) {
            }
            @Override
            public void windowIconified(WindowEvent e) {
            }
            @Override
            public void windowDeiconified(WindowEvent e) {
            }
            @Override
            public void windowActivated(WindowEvent e) {
            }
            @Override
            public void windowDeactivated(WindowEvent e) {
            }
        });
        frame.setContentPane(panel);
        frame.setVisible(true);

        // connect with Server
        socket = new Socket("192.168.251.169", 6000);
        sender = new PrintWriter(socket.getOutputStream());
        recver = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        System.out.println("connect successfully!");
    }

    /**
     * open terminal
     * ready to receive command from user
     * send command to Server
     * and receive result from Server
     * */
    public void openTerminal() {
        try {
            Scanner scan = new Scanner(System.in);
            while (true) {
                System.out.print(">");
                String cmd = scan.nextLine();
                if (cmd.contains("exit")) {// If the user enters "exit", close terminal
                    break;
                }
                sender.print(cmd);// send command to Server
                sender.flush();
                StringBuilder result = new StringBuilder();
                // receive from Server line by line
                while (true) {
                    String line = recver.readLine();
                    if (line.contains("ERESP")) break;// Server sends "ERESP" indicating that there are no more results to receive
                    result.append(line).append("\n");
                }
                System.out.println(result);// print all results
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * set LED icon according to LEDStatus
     * */
    public void setLEDStatus(int[] LEDStatus) {
        for (int i=0; i<16; i++) {
            LEDs[i].setIcon(LEDStatus[i] == 1 ? LEDOnIcon : LEDOffIcon);
        }
    }

    /**
     * Upload the file to the server
     * compile the file
     * and receive the compilation information
     * */
    public void uploadFile() {
        try {
            sender.print("upload");
            sender.flush();
            System.out.println(recver.readLine());
            // upload file line by line
            Scanner fileScan = new Scanner(new FileReader(filePath));
            while (fileScan.hasNextLine()) {
                String line = fileScan.nextLine();
                System.out.println(line);
                sender.print(line + "\n");
                sender.flush();
            }
            sender.print("EREQ");// Client Send "EREQ" indicating that there are no more data to send
            sender.flush();
            System.out.println("recv: " + recver.readLine());
            // send command to compile the file
            sender.print("make vmodel");
            sender.flush();
            StringBuilder result = new StringBuilder();
            enableRun = true;
            // receive the compilation information line by line
            while (true) {
                String line = recver.readLine();
                if (line.contains("error")) enableRun = false;// Whether there is an error in the code
                if (line.contains("ERESP")) break;// Server sends "ERESP" indicating that there are no more results to receive
                result.append(line).append("\n");
            }
            runBtn.setEnabled(enableRun);// enable/disable run button
            System.out.println(result);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * another thread to run FPVGA
     * Use multithreading to ensure that the GUI is not blocked
     *
     * Workflow:
     * Client-------------------------- Server
     * "fpvga"                  =>
     *                          <=      Whether to enable FPVGA
     * LOOP:
     * clk|rst|switchStatus     =>
     *                          <=      LED status
     * goto LOOP
     * */
    @Override
    public void run() {
        try {
            boolean clk = true;
            // Client send "fpvga" to Server
            // to enable fpvge
            sender.print("fpvga");
            sender.flush();
            System.out.println(recver.readLine());
            // Client send clk|rst|switchStatus to Server
            // Server send LEDstatus to Client
            while (true) {
                clk = !clk;// The clock signal flips automatically
                if (!isRun) {// End of the run
                    sender.print("EREQ");// send "EREQ" indicates the end of the FPVGA
                    sender.flush();
                    System.out.println(recver.readLine());
                    break;
                }
                // Client send clk|rst|switchStatus to Server
                StringBuilder switchStatusSequence = new StringBuilder();
                for (int i=0; i<16; i++) {
                    switchStatusSequence.append(switchStatus[i]);
                }
                System.out.println(isRst);
                System.out.println("send: " + (clk ? "1" : "0") + " " + (isRst ? "0" : "1") + " " + switchStatusSequence);
                sender.print((clk ? "1" : "0") + (isRst ? "0" : "1") + switchStatusSequence);
                sender.flush();
                // Server send LEDstatus to Client
                String LEDStatusSequence = recver.readLine();
                System.out.println("recv LED: " + LEDStatusSequence);
                int[] LEDStatus = new int[16];
                for (int i=0; i<16; i++) {
                    LEDStatus[i] = LEDStatusSequence.charAt(i) - '0';
                    System.out.print(LEDStatus[i]);
                }
                setLEDStatus(LEDStatus);// change LED status
                // It can be considered that the clock frequency of FPVGA is 10 milliseconds by default
                // (If the network delay is ignored)
                sleep(10);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) throws IOException {
        new Client();
    }
}
