import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;

import javax.swing.JComponent;

public class DigitalTube extends JComponent {
    // The status of each segment of the digital tube.
    // The highest bit of 1 indicates that the a-segment is enabled
    public String input;
    // The X coordinate of the starting position of the drawing
    public int X = 10;
    // The Y coordinate of the starting position of the drawing
    public int Y = 30;
    // The width of the figure
    public int WIDTH = 50;
    // Line width
    public int LINEWIDTH = 10;

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setStroke(new BasicStroke(LINEWIDTH, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
        displayDigital(input, g2d);
    }

    /**
     * Draw a line segment
     * @param x1: The x-coordinate of the line segment start point
     * @param y1: The y-coordinate of the line segment start point
     * @param x2: The x-coordinate of the line segment end point
     * @param y2: The y-coordinate of the line segment end point
     * @param color: The color of the line segment
     * @param g2d: Graphics2D object
     */
    public void drawLine(int x1, int y1, int x2, int y2, Color color, Graphics2D g2d) {
        g2d.setColor(color);
        Line2D line = new Line2D.Double(x1, y1, x2, y2);
        g2d.draw(line);
    }

    /**
     * Draw "8" according to the given color
     * @param colors: The color of each segment
     * @param g2d: Graphics2D object
     *  -----a------
     * |            |
     * f            b
     * |            |
     *  -----f------
     * |            |
     * e            c
     * |            |
     *  -----d------g
     */
    public void drawDigital(Color[] colors, Graphics2D g2d) {
        drawLine(X + LINEWIDTH, Y, X + WIDTH - LINEWIDTH, Y, colors[0], g2d);// a
        drawLine(X + WIDTH, Y + LINEWIDTH, X + WIDTH, Y + WIDTH - LINEWIDTH, colors[1], g2d);// b
        drawLine(X + WIDTH, Y + WIDTH + LINEWIDTH, X + WIDTH, Y + 2 * WIDTH - LINEWIDTH, colors[2], g2d);// c
        drawLine(X + LINEWIDTH, Y + 2 * WIDTH, X + WIDTH - LINEWIDTH, Y + 2 * WIDTH, colors[3], g2d);// d
        drawLine(X, Y + WIDTH + LINEWIDTH, X, Y + 2 * WIDTH - LINEWIDTH, colors[4], g2d);// e
        drawLine(X, Y + LINEWIDTH, X, Y + WIDTH - LINEWIDTH, colors[5], g2d);//e
        drawLine(X + LINEWIDTH, Y + WIDTH, X + WIDTH - LINEWIDTH, Y + WIDTH, colors[6], g2d);//f
        drawLine(X + WIDTH + 3, Y + 2 * WIDTH + 3, X + WIDTH + 3, Y + 2 * WIDTH + 3, colors[7], g2d);//dot
    }

    /**
     * Determine the color of each segment according to the input.
     * @param input: The status of each segment of the digital tube. The highest bit of 1 indicates that the a-segment is enabled
     * @param g2d: Graphics2D object
     */
    public void displayDigital(String input, Graphics2D g2d) {
        Color[] colors = new Color[8];
        Color gray = new Color(0xdddddd);
        Color red = new Color(0xd81e06);
        if (input == null || input.trim().equals("")) {
            colors[0] = gray;
            colors[1] = gray;
            colors[2] = gray;
            colors[3] = gray;
            colors[4] = gray;
            colors[5] = gray;
            colors[6] = gray;
            drawDigital(colors, g2d);
            return;
        }
        for (int i=0; i<input.length(); i++) {
            colors[i] = input.charAt(i) == '1' ? red : gray;
        }
        drawDigital(colors, g2d);
    }
}
