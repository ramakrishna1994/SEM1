import java.awt.List;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.GenericDeclaration;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

public class SentimentAnalysis {
	
	public static final String POSITIVE = "positive";
	public static final String NEGATIVE = "negative";
	public static HashMap<String, Integer[]> allUniqueWordsMap = new HashMap<>();
	public static HashMap<String, float[]> ProbabilitesOfAllUniqueWordsMap = new HashMap<>();
	public static float probabilityOfPositive = 0;
	public static float probabilityOfnegative = 0;
	/*public static String postiveReviewsDirectory = "actualtrainingdata/pos";
	public static String negativeReviewsDirectory = "actualtrainingdata/neg";*/
	
	public static String postiveTrainingReviewsDirectory = "train/pos";
	public static String negativeTrainingTrReviewsDirectory = "train/neg";
	public static String postiveTestReviewsDirectory = "test/pos";
	public static String negativeTestTrReviewsDirectory = "test/neg";
	
	public static int GetNoOfReviewsOfSpecificClass(File folder)
	{
		return folder.listFiles().length;
	}
	
	public static void listFilesForFolder(final File folder,HashMap<String, Integer[]> wordMap,String whichClass) 
	{
		
		
		int i=1;
	    for (final File fileEntry : folder.listFiles()) {
	        if (fileEntry.isDirectory()) {
	            listFilesForFolder(fileEntry,wordMap,whichClass);
	        } else {
	        	try {
					//System.out.println(fileEntry.getCanonicalPath().toString());
	        		String fileName = fileEntry.getCanonicalPath().toString();
	        		readDataFromFileAndSplitToWords(fileName,wordMap,whichClass);
					System.out.println("Completed reading doc : "+i);
					i++;
				} catch (IOException e) {
					e.printStackTrace();
				}
	        	
	            
	        }
	    }
	}
	
	public static void insertIntoWordMap(String word,HashMap<String, Integer[]> wordMap,String whichClass)
	{
		if(!wordMap.containsKey(word) && whichClass.equals(POSITIVE))
		{
			Integer[] counts = new Integer[] {1,0};
			wordMap.put(word, counts);
		}
		else if(!wordMap.containsKey(word) && whichClass.equals(NEGATIVE))
		{
			Integer[] counts = new Integer[] {0,1};
			wordMap.put(word, counts);
		}
		else if(wordMap.containsKey(word) && whichClass.equals(POSITIVE))
		{
			Integer[] counts = new Integer[] {0,0};
			counts[0] = wordMap.get(word)[0] + 1;
			counts[1] = wordMap.get(word)[1];
			wordMap.replace(word, counts);
		}
		else if(!wordMap.containsKey(word) && whichClass.equals(NEGATIVE))
		{
			Integer[] counts = new Integer[] {0,0};
			counts[0] = wordMap.get(word)[0];
			counts[1] = wordMap.get(word)[1] + 1;
			wordMap.replace(word, counts);
		}
	}

	public static void readDataFromFileAndSplitToWords(String filePath,HashMap<String, Integer[]> wordMap,String whichClass) throws IOException
	{
		BufferedReader br = new BufferedReader(new FileReader(filePath));
	    try {
	        String line = br.readLine();

	        while (line != null) {
	        	String speech = line;
	        	String[] result = speech.split("\\s|\\.|,|-|\\(|\\)|--|\\{|\\}|/");
	        	for (int x=0; x<result.length; x++) {
	        		  //System.out.println(result[x]);
	        			String modifiedString = result[x].toLowerCase().replaceAll("[-\\\\[\\\\]^/,'*:;,_.!><~@#$%+=?|\\\"\\\\\\\\()]+", "");
	        			if(!modifiedString.isEmpty())
	        				insertIntoWordMap(modifiedString,wordMap,whichClass);  
	        		}
	            line = br.readLine();
	        }
	    } finally {
	        br.close();
	    }
	}
	
	public static void printCountsMap(HashMap<String, Integer[]> wordMap)
	{
		 Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
		    while (it.hasNext()) {
		        Entry<String, Integer[]> pair = it.next();
		        System.out.println(pair.getKey() + " = " + pair.getValue()[0] + "," + pair.getValue()[1]);
		    }
	}
	
	public static void printProbabilitiesMap(HashMap<String, float[]> wordMap)
	{
		 Iterator<Entry<String, float[]>> it = wordMap.entrySet().iterator();
		    while (it.hasNext()) {
		        Entry<String, float[]> pair = it.next();
		        System.out.println(pair.getKey() + " = " + pair.getValue()[0] + "," + pair.getValue()[1]);
		    }
	}
	
	
	public static void BuildCountsForWords()
	{
		File posFolder = new File(postiveTrainingReviewsDirectory);
		File negFolder = new File(negativeTrainingTrReviewsDirectory);
		listFilesForFolder(negFolder,allUniqueWordsMap,NEGATIVE);
		listFilesForFolder(posFolder,allUniqueWordsMap,POSITIVE);
	}
	
	
	public static int getTotalNoOfWordsInSpecificClass(HashMap<String, Integer[]> wordMap,String whichClass)
	{
		Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
		int totalNoOfWordsInSpecificClass = 0;
	    while (it.hasNext()) {
	        Entry<String, Integer[]> pair = it.next();
	        if(whichClass.equals(POSITIVE))
	        	totalNoOfWordsInSpecificClass += pair.getValue()[0];
	        if(whichClass.equals(NEGATIVE))
	        	totalNoOfWordsInSpecificClass += pair.getValue()[1];
	    }
	    return totalNoOfWordsInSpecificClass;
	}
	
	public static void BuildProbabilitiesForWords(HashMap<String, Integer[]> wordMap,int noOfTotalWordsInPositiveClass,int noOfTotalWordsInNegativeClass,int vocabulary)
	{
		Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
	    while (it.hasNext()) {
	        Entry<String, Integer[]> pair = it.next();
	        String key = pair.getKey();
	        float probabilityOfWordGivenPositive = (float)(pair.getValue()[0] + 1)/(float)(noOfTotalWordsInPositiveClass + vocabulary);
	        float probabilityOfWordGivenNegative = (float)(pair.getValue()[1] + 1)/(float)(noOfTotalWordsInNegativeClass + vocabulary);
	        float[] probabilities = new float[] {probabilityOfWordGivenPositive,probabilityOfWordGivenNegative};
	        ProbabilitesOfAllUniqueWordsMap.put(key, probabilities);
	    }
	}
	
	
	public static int getNoOfWordsInVocabulary(HashMap<String, Integer[]> wordMap)
	{
		return wordMap.size();
	}
	
	public static void StartAlgo()
	{
		BuildCountsForWords();
		int noOfPositiveReviews = GetNoOfReviewsOfSpecificClass(new File(postiveTrainingReviewsDirectory));
		int noOfNegativeReviews = GetNoOfReviewsOfSpecificClass(new File(negativeTrainingTrReviewsDirectory));
		int noOfTotalWordsInPositiveClass = getTotalNoOfWordsInSpecificClass(allUniqueWordsMap,POSITIVE);
		int noOfTotalWordsInNegativeClass = getTotalNoOfWordsInSpecificClass(allUniqueWordsMap,NEGATIVE);
		int noOfUniqueWordsInVocabulary = getNoOfWordsInVocabulary(allUniqueWordsMap);
		System.out.println(noOfTotalWordsInPositiveClass + " " + noOfTotalWordsInNegativeClass);
		BuildProbabilitiesForWords(allUniqueWordsMap,noOfTotalWordsInPositiveClass,noOfTotalWordsInNegativeClass,noOfUniqueWordsInVocabulary);
		int totalNoOfReviews = noOfPositiveReviews + noOfNegativeReviews;
		probabilityOfPositive = (float)noOfPositiveReviews / (float)totalNoOfReviews;
		probabilityOfnegative = (float)noOfNegativeReviews / (float)totalNoOfReviews;
		System.out.println(probabilityOfPositive + " " + probabilityOfnegative);
	}
	
	
	public static void main(String []args)
	{
		
		StartAlgo();
		printCountsMap(allUniqueWordsMap);
		printProbabilitiesMap(ProbabilitesOfAllUniqueWordsMap);
		
	}
	
	
}
