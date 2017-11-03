import java.math.BigDecimal;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

public class dummy {

	public static void main(String args[])
	{
		String text = ">/great:?<;!_\"";
		System.out.println(text.replaceAll("[-\\\\[\\\\]^/,'*:;,_.!><~@#$%+=?|\\\"\\\\\\\\()]+", ""));
		HashMap<String, Integer> dummy = new HashMap<>();
		dummy.put("abc", 1);
		dummy.put("def", 2);
		dummy.put("ert", 3);
		
		for(int i=1;i<=100000;i++)
			System.out.print(i/1000 + "%\r");
		System.out.println();
		
		double rk = 1.294881398575501E-6;
		double rk1 = 4.228784980240717E-263;
		
		BigDecimal a1 = new BigDecimal(rk);
		BigDecimal a2 = new BigDecimal(rk1);
		System.out.println(a2);
		if(a1.compareTo(a2) == 1)
			System.out.println(true);
		if(a1.compareTo(a2) == -1)
			System.out.println(false);
		
	}
}
