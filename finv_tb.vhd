-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use IEEE.std_logic_textio.all;

use std.textio.all;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS

  -- Component Declaration
  COMPONENT finv
    PORT(
      clk : in std_logic;
      i : IN std_logic_vector(31 downto 0);
      o : OUT std_logic_vector(31 downto 0));
  END COMPONENT;

  SIGNAL i1 :  std_logic_vector(31 downto 0);
  SIGNAL o1 :  std_logic_vector(31 downto 0);
  signal clk : std_logic;
BEGIN

  -- Component Instantiation
  uut: finv PORT MAP(
    clk => clk,
    i => i1,
    o => o1);

  --  Test Bench Statements
  tb : PROCESS
    file infile :text open read_mode is "/home/morihiro/2014-winter/fpu/finv/testcase.txt";
    file outfile :text open write_mode is "/home/morihiro/2014-winter/fpu/finv/sim.txt";

    variable my_line, out_line : LINE;
    variable a,b : std_logic_vector(31 downto 0);
  BEGIN

    wait for 40 ns; -- wait until global set/reset completes

    -- Add user defined stimulus here

    while not endfile(infile) loop
      readline(infile, my_line);
      read(my_line, a);

      i1 <= a;

      wait for 42 ns;

      b := o1;

      write(out_line, b);
      writeline(outfile, out_line);
    end loop;

  END PROCESS;
  clkgen:process
  begin
    clk <= '0';
    wait for 7ns;
    clk <= '1';
    wait for 7ns;
  end process;

  --  End Test Bench

end behavior;
