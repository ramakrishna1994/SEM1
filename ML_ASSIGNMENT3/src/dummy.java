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
		
		Iterator<Entry<String, Integer>> it = dummy.entrySet().iterator();
	    while (it.hasNext()) {
	        Entry<String, Integer> pair = it.next();
	        System.out.println(pair.getKey());
	    }
	    

		Iterator<Entry<String, Integer>> it1 = dummy.entrySet().iterator();
	    while (it1.hasNext()) {
	        Entry<String, Integer> pair = it.next();
	        System.out.println(pair.getKey());
	    }
	}
}
