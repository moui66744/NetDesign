import javax.swing.*;
import java.awt.*;

public class DigitalTubeController  {
    public JFrame frame;
    public DigitalTube digitalTube;
    public DigitalTubeController() {
        frame = new JFrame();
        JPanel mainPanel = new JPanel();
        mainPanel.setBackground(Color.white);
        digitalTube = new DigitalTube();
        digitalTube.setBounds(10, 0, 80, 200);
        mainPanel.setLayout(null);
        mainPanel.add(digitalTube);
        frame.getContentPane().add(mainPanel);
        frame.setTitle("DigitalTubeController");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setBounds(300, 200, 300, 200);
        frame.setResizable(false);
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        DigitalTubeController digitalTubeController = new DigitalTubeController();
        digitalTubeController.digitalTube.input = "0000010";
    }
}