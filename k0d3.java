/**
 * One file programming language.
 * Should be turing complete?
 * 
 * @author Justus Languell
 */

import java.io.File;
import java.io.IOException;
import java.lang.NumberFormatException;
import java.util.Scanner;

import java.util.HashMap;


public class k0d3 {

  private static HashMap<String, Double> stack = 
    new HashMap<String, Double>();

  private static Scanner scanner = new Scanner(System.in);

  public static void main(String[] args) {
    if (args.length < 1) {
      error(-1, "File not specified!\n");
    } else {
      String[] cmdargs = new String[args.length - 1];
      for (int i = 1; i < args.length; i++) {
        cmdargs[i-1] = args[i];
      }
      String fn = args[0];
      String content = readFile(fn);
      if (content != null) {
        execute(content);
      }
    }
  }

  /**
   * Executes the code in string content.
   * @param content Source code
   */
  public static void execute(String content) {
    content = removeComments(content);
    content = content.replaceAll("\n", "");
    String[] lines = content.split(";");

    String[] indexs = new String[lines.length];
    for (int i = 0; i < lines.length; i++) {
      indexs[i] = lines[i].split("")[0];
    }

    int n = 0;
    while (n < lines.length) {
      String[] parts = lines[n].split(" ");

      // FLOW CONTROL

      // Goto if
      if (parts[1].equals("$")) {
        double condition = resolveSymbol(parts[2]);
        if (condition > 0) {
          String symbol = parts[3];
          if (symbol.charAt(0) == '$') {
            symbol = symbol.substring(1);
            for (int i = 0; i < indexs.length; i++) {
              if (indexs[i].equals(symbol)) {
                n = i - 1;
              }
            }
          }
        }
      }

      // NUMERICAL OPERATIONS

      // Add
      if (parts[1].equals("+")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], a + b);
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Subtract
      if (parts[1].equals("-")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], a - b);
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Multiply
      if (parts[1].equals("*")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], a * b);
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Divide
      if (parts[1].equals("/")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], a / b);
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Modulo
      if (parts[1].equals("%")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], a % b);
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Power
      if (parts[1].equals("^")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          stack.put(parts[4], Math.pow(a, b));
        } else {
          error(n, "Invalid operation!\n");
        }
      }

      // Round
      if (parts[1].equals("@")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '#') {
          double scale = Math.pow(10, b);
          stack.put(parts[4], (double) Math.round(a * scale) / scale);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // BOOLEAN OPERATIONS

      // Equal
      if (parts[1].equals("=")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (a == b) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // Less Than
      if (parts[1].equals("<")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (a < b) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // Greater Than
      if (parts[1].equals(">")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (a > b) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // Not
      if (parts[1].equals("!")) {
        double a = resolveSymbol(parts[2]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (!(a > 0)) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // And
      if (parts[1].equals("&")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (a > 0 && b > 0) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // Or
      if (parts[1].equals("|")) {
        double a = resolveSymbol(parts[2]);
        double b = resolveSymbol(parts[3]);
        if (parts[4].charAt(0) == '?') {
          stack.put(parts[4], (a > 0 || b > 0) ? 1.0 : 0.0);
        } else {
          error(n, "Invalid operation!\n");
          
        }
      }

      // INPUT / OUTPUT

      // Write Ascii to console
      if (parts[1].equals("\"")) {
        for (int i = 2; i < parts.length; i++) {
          if (parts[i].startsWith(".")) {
            String s = (resolveSymbol(parts[i])) > 0 ? "true" : "false";
            System.out.printf("%s", s);
          } else if (parts[i].startsWith("#")) {
            char c = (char)(int)resolveSymbol(parts[i]);
            System.out.printf("%c", c);
          } else if (parts[i].startsWith(":")) {
            //
          } else {
            // 
          }
        }
      } 

      // Write double to console
      if (parts[1].equals("#")) {
        for (int i = 2; i < parts.length; i++) {
          System.out.printf("%f", resolveSymbol(parts[i]));
        }
      } 

      // Reads double from console
      if (parts[1].equals("?")) {
        try {
          double in = Double.parseDouble(scanner.nextLine());
          if (parts[2].charAt(0) == '#') {
            stack.put(parts[2], in);
          } else {
            error(n, "Invalid input buffer!\n");
          }
        } catch (NumberFormatException e) { 
          error(n, "Invalid number input!\n");
        }
      }
      n++;
    }
  }

  /**
   * Read source code from file as string.
   * 
   * @param filename The name of source file.
   * @return The string content of the file.
   */
  public static String readFile(String filename) {
    try {
      String content = "";
      File fObj = new File(filename);
      Scanner reader = new Scanner(fObj);
      while (reader.hasNext()) {
        content += String.format("%s\n", reader.nextLine());
      }
      reader.close();
      return content;
    } catch (IOException e) {
      error(-1, "File could not be found!\n");
      return null;
    }
  }

  /**
   * Resolve a symbol into a value.
   * 
   * @param symbol Symbol
   * @return Value of symbol
   */
  static double resolveSymbol(String symbol) {
    double val = 0.0;
    if (symbol.startsWith("#")) {
      if (stack.containsKey(symbol)) {
        val = stack.get(symbol);
      } else {
        stack.put(symbol, 0.0);
        val = 0.0;
      }
    } else {
      try {
        val = Double.parseDouble(symbol);
      } catch (NumberFormatException e) {
        val = 0.0;
      }
    }
    return val;
  }

  /**
   * Removes comments from source string.
   * 
   * @param content Source code string.
   * @return Source code string with comments removed.
   */
  static String removeComments(String content) {
    boolean comment = false;
    String ret = "";
    for (int i = 0; i < content.length(); i++) {
      char c = content.charAt(i);
      if (c == '(') {
        comment = true;
      } else if (c == ')') {
        comment = false;
      } else {
        if (!comment) {
          ret += c;  
        }
      }
    }
    return ret;
  }

  /**
   * Prints a red error message.
   * 
   * HOLY SHIT COPILOT IS GOOD! 
   * 
   * @param s Error message.
   */
  public static void error(int n, String s) {
    if (n >= 0) {
      System.out.printf("\u001b[31mError: \"%s\" on line %d\u001b[0m", s, n);
    } else {
      System.out.printf("\u001b[31mError: \"%s\"\u001b[0m", s);
    }
    System.exit(0);
  }
}