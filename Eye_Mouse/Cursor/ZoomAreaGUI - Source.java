import javax.swing.*;
import java.awt.*;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagLayout;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class ZoomAreaGUI {
    
    private String _window_name;
    
    private int _area_width;
    private int _area_height; 
    
    private int _border_thickness;
    
    private Color _border_color;
    
    public static void main(String[] args) {    
	try{
		if (args.length != 5)
			throw new Exception("Invalid number of arguments!");
        
		int area_width = Integer.parseInt(args[0]);
		int area_height = Integer.parseInt(args[1]);
		int border_thickness = Integer.parseInt(args[2]);
		Color border_color = getColor(args[3]);
        
		new ZoomAreaGUI(area_width, area_height, border_thickness, border_color, args[4]/* Window name */);
	}
	catch (Exception e){
		JOptionPane.showMessageDialog(null, "Error Creating Zoom Area GUI: " + e.getMessage());
	}
    }

    public ZoomAreaGUI(int area_width, int area_height, int border_thickness, Color border_color, String window_name) {
		_area_width = area_width;
		_area_height = area_height;
		_border_thickness = border_thickness;
		_border_color = border_color;
		_window_name = window_name;
     
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
               
                try {
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                } catch (ClassNotFoundException ex) {
                    Logger.getLogger(ZoomAreaGUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (InstantiationException ex) {
                    Logger.getLogger(ZoomAreaGUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IllegalAccessException ex) {
                    Logger.getLogger(ZoomAreaGUI.class.getName()).log(Level.SEVERE, null, ex);
                } catch (UnsupportedLookAndFeelException ex) {
                    Logger.getLogger(ZoomAreaGUI.class.getName()).log(Level.SEVERE, null, ex);
                }
             
                JFrame frame = new JFrame(_window_name);           
                frame.setUndecorated(true);
                frame.setBackground(new Color(0, 0, 0, 0));
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.add(new TestPane());
                frame.setAlwaysOnTop(true);
                frame.pack();
                frame.setLocationRelativeTo(null);
                frame.setVisible(true);               
            }
        });
    }
    
    public class TestPane extends JPanel {

        public TestPane() {
            setOpaque(false);
            setLayout(new GridBagLayout());
        }

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(_area_width, _area_height);
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2d = (Graphics2D) g.create();
            g2d.setColor(getBackground());
            g2d.setStroke(new BasicStroke(_border_thickness));
            g2d.setColor(_border_color);
            g2d.drawRect(0, 0, getWidth() - 1, getHeight() - 1);
            g2d.dispose();
        }

    }
    
    private static Color getColor(String s) throws Exception { 
	s = s.toLowerCase();
      
	if (!(s.startsWith("rgb(") && s.endsWith(")")))
		throw new Exception("Invalid border color values!");
                        
	String[] colors = s.substring(4, s.length() - 1 ).split(",");
	if (colors.length != 3)
		throw new Exception("Invalid border color values!");
      
	Color color = new Color(
		Integer.parseInt(colors[0].trim()),
            	Integer.parseInt(colors[1].trim()),
            	Integer.parseInt(colors[2].trim())
            	);
      
	return color;
    }
}