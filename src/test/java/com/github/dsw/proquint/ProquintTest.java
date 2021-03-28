package com.github.dsw.proquint;

import static org.junit.Assert.assertEquals;

import java.util.HashSet;
import java.util.Set;

import org.junit.Test;

public class ProquintTest {

  @Test
  public void should_encode_hex() throws Exception {
    assertEquals("lusab-babad", Proquint.encode(0x7f000001));
    assertEquals("gutih-tugad", Proquint.encode(0x3f54dcc1));
  }
  
  @Test
  public void should_encode_integer() throws Exception {
    assertEquals("lusab-babad", Proquint.encode(2130706433));
    assertEquals("gutih-tugad", Proquint.encode(1062526145));
  }
  
  @Test
  public void should_decode() throws Exception {
    assertEquals("0xffffffff", Proquint.decode("zuzuz-zuzuz"));
  }
  
  @Test
  public void should_encode_longs() throws Exception {
    assertEquals("zuzuz-zuzuz", Proquint.encode(Long.parseLong("ffffffff", 16)));
  }
  
  @Test
  public void should_generate_random_proquints() throws Exception {
    Set<String> proquints = new HashSet<String>();
    
    for (int i = 0; i < 100; i++) {
      proquints.add(Proquint.randomProquint());
    }
    
    assertEquals(100, proquints.size());
  }
}
