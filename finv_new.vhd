library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;
library std;
use std.textio.all;
entity finv_table is
  port(
    clk : in std_logic;
    en  : in std_logic;
    addr : in unsigned(10 downto 0);
    data : out unsigned(35 downto 0));
end finv_table;
architecture blackbox of finv_table is
    type ramtype is array(0 to 2047) of unsigned(35 downto 0);
  impure function InitRamFromFile (RamFileName : string) return ramtype is
    file ramFile : text open read_mode is RamFileName;
    variable ramFileLine : line;
    variable ram : ramtype;
    variable temp : std_logic_vector(35 downto 0);
  begin
    for I in ramtype'range loop
      readline (ramFile, ramFileLine);
      read (ramFileLine,temp);
      ram(I) := unsigned(temp);
    end loop;
    return ram;
  end function;
  signal ram : ramtype := InitRamFromFile("/home/morihiro/2014-winter/fpu/finv/finv_table2048.txt");

begin
  process (CLK)
  begin
    if rising_edge(clk) then
      if en = '1' then
        data <= ram(to_integer(addr));
      end if;
    end if;
  end process;
end blackbox;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity finv is
  port(
    clk : in std_logic;
    i   : in std_logic_vector(31 downto 0);
    o   : out std_logic_vector(31 downto 0));
end finv;
architecture blackbox of finv is
  component finv_table is
    port(
      clk : in std_logic;
      en  : in std_logic;
      addr : in unsigned(10 downto 0);
      data : out unsigned(35 downto 0));
  end component;
  signal en : std_logic := '1';
  signal addr : unsigned(10 downto 0);
  signal data : unsigned(35 downto 0);
  signal state : std_logic_vector(1 downto 0) := "00";
  signal ui : unsigned(31 downto 0);
  signal frac : unsigned(22 downto 0);
  signal sign : unsigned(0 downto 0);
  signal exp : unsigned(7 downto 0);
  signal ans : unsigned(31 downto 0);
  signal temp : unsigned(35 downto 0);
begin
  table : finv_table port map(
    clk => clk,
    en => en,
    addr => addr,
    data => data);
  process(clk)
    variable y : unsigned(23 downto 0);
    variable d : unsigned(12 downto 0);
    variable manti : unsigned(24 downto 0);
    variable uiv : unsigned(31 downto 0);
  begin
    addr <= unsigned(i(22 downto 12));
    if rising_edge(clk) then
      case state is
        when "00" =>
          state <= "01";
          ui <= unsigned(i);
        when "01" =>
          y := "1" & (data(35 downto 13));
          d := data(12 downto 0);
          uiv := ui;
          manti := y + shift_right(d * (4096 - uiv(11 downto 0)),12);
          frac <= manti(22 downto 0);
          sign <= uiv(31 downto 31);
          exp <= 253 - uiv(30 downto 23);
          state <= "10";
        when "10" =>
          o <= std_logic_vector(sign & exp & frac);
          state <= "00";
        when others =>
          temp <= data;
          state <= "01";
      end case;
    end if;
  end process;
end blackbox;
