import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.*;
import java.net.Socket;
import java.util.Arrays;
import java.util.Scanner;

import static java.lang.Thread.sleep;

public class Client implements Runnable {
    public JFrame frame;
    public JPanel panel;
    public JPanel panel1;
    public JPanel panel2;
    public JPanel panel3;
    public JLabel[] LEDs;
    public JToggleButton[] toggleBtns = new JToggleButton[16];
    public JButton chooseFileBtn;
    public JToggleButton runBtn;
    public JButton openTerminalBtn;
    public File filePath = new File("C:\\Users\\86187\\Desktop\\a.v");
    public int[] switchStatus = new int[16];

    public Thread runThread;
    public boolean isRun;
    public boolean enableRun;

    public ImageIcon LEDOnIcon;
    public ImageIcon LEDOffIcon;
    public ImageIcon switchOnIcon;
    public ImageIcon switchOffIcon;
    public ImageIcon runIcon;
    public ImageIcon terminalIcon;
    public ImageIcon uploadIcon;
    public Font font;

    public Socket socket;
    public PrintWriter sender;
    public BufferedReader recver;

    Client() throws IOException {
        frame = new JFrame("FPGA模拟器");
        frame.setSize(1000, 400);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        panel = new JPanel(new GridLayout(3, 1));
        panel.setBackground(Color.white);
        panel1 = new JPanel(new GridLayout(0, 16));
        panel1.setBackground(Color.white);
        panel2 = new JPanel(new GridLayout(0, 16));
        panel2.setBackground(Color.white);
        panel3 = new JPanel(new FlowLayout());
        panel3.setBackground(Color.white);

        LEDOnIcon = new ImageIcon("resources/LED_on.png");
        LEDOffIcon = new ImageIcon("resources/LED_off.png");
        switchOnIcon = new ImageIcon("resources/switch_on.png");
        switchOffIcon = new ImageIcon("resources/switch_off.png");
        runIcon = new ImageIcon("resources/run.png");
        terminalIcon = new ImageIcon("resources/terminal.png");
        uploadIcon = new ImageIcon("resources/terminal.png");
        font = new Font("console", Font.BOLD, 14);

        // 创建LED
        LEDs = new JLabel[16];
        for (int i=0; i<16; i++) {
            LEDs[i] = new JLabel();
            LEDs[i].setIcon(LEDOffIcon);
            panel1.add(LEDs[i]);
        }
        // 创建开关
        for (int i=0; i<16; i++) {
            toggleBtns[i] = new JToggleButton();
            // 首先设置不绘制按钮边框
            toggleBtns[i].setBorderPainted(false);
            toggleBtns[i].setFocusPainted(false);
            toggleBtns[i].setContentAreaFilled(false);
            toggleBtns[i].setBackground(Color.white);
            toggleBtns[i].setSelectedIcon(switchOnIcon);
            toggleBtns[i].setIcon(switchOffIcon);
            // 添加 toggleBtns 的状态被改变的监听
            toggleBtns[i].addActionListener(e -> {
                JToggleButton toggleBtn1 = (JToggleButton) e.getSource();
                switchStatus[Arrays.asList(toggleBtns).indexOf(toggleBtn1)] = toggleBtn1.isSelected() ? 1 : 0;
            });
            panel2.add(toggleBtns[i]);
        }

        // 上传文件按钮
        chooseFileBtn = new JButton("choose file");
        chooseFileBtn.setFont(font);
        chooseFileBtn.setPreferredSize(new Dimension(140, 100));
        chooseFileBtn.setBackground(new Color(0xf6f6f6));
        chooseFileBtn.setIcon(uploadIcon);
        chooseFileBtn.setHorizontalTextPosition(SwingConstants.CENTER);
        chooseFileBtn.setVerticalTextPosition(SwingConstants.BOTTOM);
        chooseFileBtn.addActionListener(e -> {
            // 创建一个默认的文件选取器
            JFileChooser fileChooser = new JFileChooser();
            // 设置文件选择的模式（只选文件、只选文件夹、文件和文件均可选）
            fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            // 设置是否允许多选
            fileChooser.setMultiSelectionEnabled(false);
            // 设置默认使用的文件过滤器
            fileChooser.setFileFilter(new FileNameExtensionFilter("verilog(*.v)", "v"));
            // 打开文件选择框（线程将被阻塞, 直到选择框被关闭）
            int result = fileChooser.showOpenDialog(frame);
            if (result == JFileChooser.APPROVE_OPTION) {
                // 如果点击了"确定", 则获取选择的文件路径
                filePath = fileChooser.getSelectedFile();
                uploadFile();
            }
        });
        panel3.add(chooseFileBtn);

        // 运行按钮
        runBtn = new JToggleButton("run");
        runBtn.setFont(font);
        runBtn.setPreferredSize(new Dimension(140, 100));
        runBtn.setBackground(new Color(0xf6f6f6));
        runBtn.setIcon(runIcon);
        runBtn.setHorizontalTextPosition(SwingConstants.CENTER);
        runBtn.setVerticalTextPosition(SwingConstants.BOTTOM);
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

        // 打开终端按钮
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
            public void windowClosing(WindowEvent e) {
                try {
                    sender.print("shutdown");
                    sender.flush();
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

        socket = new Socket("192.168.251.169", 6000);
        sender = new PrintWriter(socket.getOutputStream());
        recver = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        System.out.println("connect successfully!");
//        openTerminal();
    }

    public void openTerminal() {
        try {
            Scanner scan = new Scanner(System.in);
            while (true) {
                System.out.print(">");
                String cmd = scan.nextLine();
                if (cmd.contains("exit")) {
                    break;
                }
                sender.print(cmd);
                sender.flush();
                StringBuilder result = new StringBuilder();
                while (true) {
                    String line = recver.readLine();
                    if (line.contains("ERESP")) break;
                    result.append(line).append("\n");
                }
                System.out.println(result);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void setLEDStatus(int[] LEDStatus) {
        for (int i=0; i<16; i++) {
            LEDs[i].setIcon(LEDStatus[i] == 1 ? LEDOnIcon : LEDOffIcon);
        }
    }

    public void uploadFile() {
        try {
            StringBuilder cmd = new StringBuilder("upload");
            sender.print(cmd);
            sender.flush();
            System.out.println(recver.readLine());
            // 逐行发送文件
            Scanner fileScan = new Scanner(new FileReader(filePath));
            while (fileScan.hasNextLine()) {
                String line = fileScan.nextLine();
                System.out.println(line);
                sender.print(line + "\n");
                sender.flush();
            }
            sender.print("EREQ");
            sender.flush();
            System.out.println("recv: " + recver.readLine());

            sender.print("make vmodel");
            sender.flush();
            StringBuilder result = new StringBuilder();
            while (true) {
                String line = recver.readLine();
                if (line.contains("ERESP")) break;
                result.append(line).append("\n");
            }
            System.out.println(result);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            StringBuilder cmd = new StringBuilder("fpvga");
            sender.print(cmd);
            sender.flush();
            System.out.println(recver.readLine());
            while (true) {
                if (!isRun) {
                    StringBuilder ereq = new StringBuilder("EREQ");
                    sender.print(ereq);
                    sender.flush();
                    System.out.println(recver.readLine());
                    break;
                }
                StringBuilder switchStatusSequence = new StringBuilder();
                for (int i=0; i<16; i++) {
                    switchStatusSequence.append(switchStatus[i]);
                }
                System.out.println("send switch: " + switchStatusSequence);
                sender.print(switchStatusSequence);
                sender.flush();
                String LEDStatusSequence = recver.readLine();
                System.out.println("recv LED: " + LEDStatusSequence);
                int[] LEDStatus = new int[16];
                for (int i=0; i<16; i++) {
                    LEDStatus[i] = LEDStatusSequence.charAt(i) - '0';
                    System.out.print(LEDStatus[i]);//
                }
                setLEDStatus(LEDStatus);
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
