package smpt_server;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextArea;
import java.awt.TextField;
import java.util.Vector;

import javax.swing.JFrame;


public class Message extends JFrame {
	private Label toLabel		=	new Label("                   To : ");
	private TextField toField	=	new TextField("", 40);
	private Label fromLabel		=	new Label("              From : ");
	private TextField fromField	=	new TextField("", 40);
	private Label subjectLabel	=	new Label("          Subject : ");
	private TextField subjectField=	new TextField("", 40);
	private Label messageLabel	=	new Label("Message : ");
	private TextArea messageText=	new TextArea(10, 40);
	private TextArea fileText=	new TextArea(5, 40);
	
	public Message(String from, String to, String subject, String message, Vector<String> files)
	{
		super("Message");
		
		fromField.setText(from);
		toField.setText(to);
		subjectField.setText(subject);
		messageText.setText(message);
		
		Panel fromPanel = new Panel(new BorderLayout());
		Panel toPanel = new Panel(new BorderLayout());
		Panel subjectPanel = new Panel(new BorderLayout());
		Panel messagePanel = new Panel(new BorderLayout());

		fromPanel.add(fromLabel, BorderLayout.WEST);
		fromPanel.add(fromField, BorderLayout.CENTER);
		toPanel.add(toLabel, BorderLayout.WEST);
		toPanel.add(toField, BorderLayout.CENTER);
		subjectPanel.add(subjectLabel, BorderLayout.WEST);
		subjectPanel.add(subjectField, BorderLayout.CENTER);
		messagePanel.add(messageLabel, BorderLayout.NORTH);	
		messagePanel.add(messageText, BorderLayout.CENTER);

		Panel fieldPanel = new Panel(new GridLayout(0, 1));
		fieldPanel.add(fromPanel);
		fieldPanel.add(toPanel);
		fieldPanel.add(subjectPanel);
		
		String tmp = "Recieve File : \r\n";
		for(int i = 0; i < files.size(); ++i)
		{
			tmp += (files.get(i) + "\r\n");
		}
		fileText.setText(tmp);
		
		add(fieldPanel, BorderLayout.NORTH);
		add(messagePanel, BorderLayout.CENTER);
		add(fileText, BorderLayout.SOUTH);
		pack();
		show();
		
		setDefaultCloseOperation( JFrame.HIDE_ON_CLOSE );
	}
}
