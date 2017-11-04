import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

public class SentimentAnalysis {

	public static final String POSITIVE = "positive";
	public static final String NEGATIVE = "negative";
	public static HashMap<String, Integer[]> allUniqueWordsMap = new HashMap<>();
	public static HashMap<String, double[]> ProbabilitesOfAllUniqueWordsMap = new HashMap<>();
	public static HashMap<String, Integer> stopWordsList = new HashMap<>();
	public static BigDecimal probabilityOfPositive = BigDecimal.valueOf(0);
	public static BigDecimal probabilityOfnegative = BigDecimal.valueOf(0);
	public static int truePositives = 0;
	public static int trueNegatives = 0;
	public static int totalNoOfPositiveReviews = 0;
	public static int totalNoOfNegativeReviews = 0;
	public static String postiveTrainingReviewsDirectory = "actualtrainingdata/pos";
	public static String negativeTrainingTrReviewsDirectory = "actualtrainingdata/neg";
	public static String postiveTestReviewsDirectory = "actualtestdata/pos";
	public static String negativeTestReviewsDirectory = "actualtestdata/neg";
	public static String stopWordsListFileName = "stopwords.txt";
	public static Boolean isBinaryNaiveBayes = false;

	/*
	 * public static String postiveTrainingReviewsDirectory = "train/pos"; public
	 * static String negativeTrainingTrReviewsDirectory = "train/neg"; public static
	 * String postiveTestReviewsDirectory = "train/pos"; public static String
	 * negativeTestReviewsDirectory = "train/neg";
	 */

	public static int GetNoOfReviewsOfSpecificClass(File folder) {
		return folder.listFiles().length;
	}

	public static void listFilesForFolder(final File folder, HashMap<String, Integer[]> wordMap, String whichClass) {
		int i = 1;
		System.out.println("Reading Training Data");
		int total = GetNoOfReviewsOfSpecificClass(folder);
		System.out.println("Total No Of Docs To Read In " + whichClass.toUpperCase() + " Class :" + total);
		for (final File fileEntry : folder.listFiles()) {
			if (fileEntry.isDirectory()) {
				listFilesForFolder(fileEntry, wordMap, whichClass);
			} else {
				try {
					String fileName = fileEntry.getCanonicalPath().toString();
					readDataFromFileAndSplitToWords(fileName, wordMap, whichClass);
					System.out.print("No Of Docs Completed : " + i + "\r");
					i++;
				} catch (IOException e) {
					e.printStackTrace();
				}

			}
		}
		System.out.println("No Of Docs Completed : " + (i - 1) + "\n");
	}

	public static void listTestFilesForFolder(final File folder, String whichClass, boolean considerStopWords) {
		int i = 1;
		int total = GetNoOfReviewsOfSpecificClass(folder);
		System.out.println("Reading Test Data");
		System.out.println("Total No Of Docs To Read In " + whichClass.toUpperCase() + " Class :" + total);
		for (final File fileEntry : folder.listFiles()) {
			if (fileEntry.isDirectory()) {
				listTestFilesForFolder(fileEntry, whichClass, considerStopWords);
			} else {
				try {
					String fileName = fileEntry.getCanonicalPath().toString();
					System.out.print("No Of Docs Completed : " + i + "\r");
					BigDecimal probabilityOfPositiveGivenReview = CalculateMainProbability(fileName, POSITIVE,
							considerStopWords);
					BigDecimal probabilityOfNegativeGivenReview = CalculateMainProbability(fileName, NEGATIVE,
							considerStopWords);
					if (probabilityOfPositiveGivenReview.compareTo(probabilityOfNegativeGivenReview) > 0) {
						if (whichClass.equals(POSITIVE))
							truePositives++;
					}
					if (probabilityOfPositiveGivenReview.compareTo(probabilityOfNegativeGivenReview) < 0) {
						if (whichClass.equals(NEGATIVE))
							trueNegatives++;
					}

					i++;
				} catch (IOException e) {
					e.printStackTrace();
				}

			}
		}
		System.out.print("No Of Docs Completed : " + (i - 1) + "\n");
		System.out.println();
	}

	public static void ReadStopWordsAndStoreThemInMap(String stopWordsListFileName) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(stopWordsListFileName));
		try {
			String line = br.readLine();

			while (line != null) {
				String modifiedStopWord = line.toLowerCase()
						.replaceAll("[-\\\\[\\\\]^/,'*:;,_.!><~@#$%+=?|\\\"\\\\\\\\()]+", "");
				if (!stopWordsList.containsKey(modifiedStopWord))
					stopWordsList.put(modifiedStopWord, 1);
				line = br.readLine();
			}
		} finally {
			br.close();
		}
	}

	private static BigDecimal CalculateMainProbability(String fileName, String whichClass, boolean considerStopWords)
			throws IOException {

		BufferedReader br = new BufferedReader(new FileReader(fileName));
		BigDecimal mainProbability = BigDecimal.valueOf(0);
		if (whichClass.equals(POSITIVE))
			mainProbability = probabilityOfPositive;
		if (whichClass.equals(NEGATIVE))
			mainProbability = probabilityOfnegative;
		try {
			String line = br.readLine();

			while (line != null) {
				String speech = line;
				String[] result = speech.split("\\s|\\.|,|-|\\(|\\)|--|\\{|\\}|/");
				for (int x = 0; x < result.length; x++) {
					String modifiedString = result[x].toLowerCase()
							.replaceAll("[-\\\\[\\\\]^/,'*:;,_.!><~@#$%+=?|\\\"\\\\\\\\()]+", "");
					if (!modifiedString.isEmpty()) {
						if (considerStopWords == true) {
							if (!stopWordsList.containsKey(modifiedString)) {
								BigDecimal probability = getProbabilityOfWord(modifiedString, whichClass);
								mainProbability = mainProbability.multiply(probability);
							}
						} else {
							BigDecimal probability = getProbabilityOfWord(modifiedString, whichClass);
							mainProbability = mainProbability.multiply(probability);
						}

					}
				}
				line = br.readLine();
			}
		} finally {
			br.close();
		}
		return mainProbability;
	}

	private static BigDecimal getProbabilityOfWord(String modifiedString, String whichClass) {

		if (ProbabilitesOfAllUniqueWordsMap.containsKey(modifiedString) && whichClass.equals(POSITIVE))
			return BigDecimal.valueOf(ProbabilitesOfAllUniqueWordsMap.get(modifiedString)[0]);
		else if (ProbabilitesOfAllUniqueWordsMap.containsKey(modifiedString) && whichClass.equals(NEGATIVE))
			return BigDecimal.valueOf(ProbabilitesOfAllUniqueWordsMap.get(modifiedString)[1]);
		else
			return BigDecimal.valueOf(1); // Word not present so we can return 1 or ignore it.
	}

	public static void insertIntoWordMap(String word, HashMap<String, Integer[]> wordMap, String whichClass) {
		if (!wordMap.containsKey(word) && whichClass.equals(POSITIVE)) {
			Integer[] counts = new Integer[] { 1, 0 };
			wordMap.put(word, counts);
		} else if (!wordMap.containsKey(word) && whichClass.equals(NEGATIVE)) {
			Integer[] counts = new Integer[] { 0, 1 };
			wordMap.put(word, counts);
		} else if (wordMap.containsKey(word) && whichClass.equals(POSITIVE)) {
			Integer[] counts = new Integer[] { 0, 0 };
			counts[0] = wordMap.get(word)[0] + 1;
			counts[1] = wordMap.get(word)[1];
			wordMap.replace(word, counts);
		} else if (wordMap.containsKey(word) && whichClass.equals(NEGATIVE)) {
			Integer[] counts = new Integer[] { 0, 0 };
			counts[0] = wordMap.get(word)[0];
			counts[1] = wordMap.get(word)[1] + 1;
			wordMap.replace(word, counts);
		}
	}

	public static void readDataFromFileAndSplitToWords(String filePath, HashMap<String, Integer[]> wordMap,
			String whichClass) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(filePath));
		HashMap<String, Integer> tempMap = new HashMap<>();
		try {
			String line = br.readLine();

			while (line != null) {
				String speech = line;
				String[] result = speech.split("\\s|\\.|,|-|\\(|\\)|--|\\{|\\}|/");
				for (int x = 0; x < result.length; x++) {
					String modifiedString = result[x].toLowerCase()
							.replaceAll("[-\\\\[\\\\]^/,'*:;,_.!><~@#$%+=?|\\\"\\\\\\\\()]+", "");
					if (!modifiedString.isEmpty())
						if (isBinaryNaiveBayes) {
							if (!tempMap.containsKey(modifiedString)) {
								insertIntoWordMap(modifiedString, wordMap, whichClass);
								tempMap.put(modifiedString, 1);
							}
						} else {
							insertIntoWordMap(modifiedString, wordMap, whichClass);
						}

				}
				line = br.readLine();
			}
			tempMap.clear();
		} finally {
			br.close();
		}
	}

	public static void printCountsMap(HashMap<String, Integer[]> wordMap) {
		Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
		while (it.hasNext()) {
			Entry<String, Integer[]> pair = it.next();
			System.out.println(pair.getKey() + " = " + pair.getValue()[0] + "," + pair.getValue()[1]);
		}
	}

	public static void printProbabilitiesMap(HashMap<String, double[]> wordMap) {
		Iterator<Entry<String, double[]>> it = wordMap.entrySet().iterator();
		while (it.hasNext()) {
			Entry<String, double[]> pair = it.next();
			System.out.println(pair.getKey() + " = " + pair.getValue()[0] + "," + pair.getValue()[1]);
		}
	}

	public static void BuildCountsForWords() {
		File posFolder = new File(postiveTrainingReviewsDirectory);
		File negFolder = new File(negativeTrainingTrReviewsDirectory);
		listFilesForFolder(posFolder, allUniqueWordsMap, POSITIVE);
		listFilesForFolder(negFolder, allUniqueWordsMap, NEGATIVE);
	}

	public static int getTotalNoOfWordsInSpecificClass(HashMap<String, Integer[]> wordMap, String whichClass) {
		Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
		int totalNoOfWordsInSpecificClass = 0;
		while (it.hasNext()) {
			Entry<String, Integer[]> pair = it.next();
			if (whichClass.equals(POSITIVE))
				totalNoOfWordsInSpecificClass += pair.getValue()[0];
			if (whichClass.equals(NEGATIVE))
				totalNoOfWordsInSpecificClass += pair.getValue()[1];
		}
		return totalNoOfWordsInSpecificClass;
	}

	public static void BuildProbabilitiesForWords(HashMap<String, Integer[]> wordMap, int noOfTotalWordsInPositiveClass,
			int noOfTotalWordsInNegativeClass, int vocabulary) {
		Iterator<Entry<String, Integer[]>> it = wordMap.entrySet().iterator();
		while (it.hasNext()) {
			Entry<String, Integer[]> pair = it.next();
			String key = pair.getKey();
			double probabilityOfWordGivenPositive = (double) (pair.getValue()[0] + 1)
					/ (double) (noOfTotalWordsInPositiveClass + vocabulary);
			double probabilityOfWordGivenNegative = (double) (pair.getValue()[1] + 1)
					/ (double) (noOfTotalWordsInNegativeClass + vocabulary);
			double[] probabilities = new double[] { probabilityOfWordGivenPositive, probabilityOfWordGivenNegative };
			ProbabilitesOfAllUniqueWordsMap.put(key, probabilities);
		}
	}

	public static int getNoOfWordsInVocabulary(HashMap<String, Integer[]> wordMap) {
		return wordMap.size();
	}

	public static void StartAlgo() throws IOException {
		BuildCountsForWords();
		int noOfPositiveReviews = GetNoOfReviewsOfSpecificClass(new File(postiveTrainingReviewsDirectory));
		int noOfNegativeReviews = GetNoOfReviewsOfSpecificClass(new File(negativeTrainingTrReviewsDirectory));
		int noOfTotalWordsInPositiveClass = getTotalNoOfWordsInSpecificClass(allUniqueWordsMap, POSITIVE);
		int noOfTotalWordsInNegativeClass = getTotalNoOfWordsInSpecificClass(allUniqueWordsMap, NEGATIVE);
		int noOfUniqueWordsInVocabulary = getNoOfWordsInVocabulary(allUniqueWordsMap);
		System.out.println("No Of unique words or VOCABULARY : " + noOfUniqueWordsInVocabulary);
		System.out.println("No Of words in positive class : " + noOfTotalWordsInPositiveClass);
		System.out.println("No Of words in negative class : " + noOfTotalWordsInNegativeClass);
		BuildProbabilitiesForWords(allUniqueWordsMap, noOfTotalWordsInPositiveClass, noOfTotalWordsInNegativeClass,
				noOfUniqueWordsInVocabulary);
		int totalNoOfReviews = noOfPositiveReviews + noOfNegativeReviews;
		probabilityOfPositive = BigDecimal.valueOf((double) noOfPositiveReviews / (double) totalNoOfReviews);
		probabilityOfnegative = BigDecimal.valueOf((double) noOfNegativeReviews / (double) totalNoOfReviews);
		System.out.println("Probability of Positive class P(+) : " + probabilityOfPositive);
		System.out.println("Probability of Positive class P(-) : " + probabilityOfnegative);
		System.out.println();
		ReadStopWordsAndStoreThemInMap(stopWordsListFileName);
		if (isBinaryNaiveBayes)
			System.out.println(
					"==================== BINARY NAIVE BAYES WITHOUT STOP WORDS =============================");
		else
			System.out.println(
					"==================== NORMAL NAIVE BAYES WITHOUT STOP WORDS =============================");
		ClassifyTestExamplesBasedOnTrainingDataWithStopWords(false);
		printConfusionMatrix();
		if (isBinaryNaiveBayes)
			System.out.println(
					"==================== BINARY NAIVE BAYES WITH STOP WORDS ================================");
		else
			System.out.println(
					"==================== NORMAL NAIVE BAYES WITH STOP WORDS ================================");
		ClassifyTestExamplesBasedOnTrainingDataWithStopWords(true);
		printConfusionMatrix();
	}

	public static void printConfusionMatrix() {
		float tp = truePositives;
		float fn = totalNoOfPositiveReviews - truePositives;
		float fp = totalNoOfNegativeReviews - trueNegatives;
		float tn = trueNegatives;
		System.out.println("       CONFUSION MATRIX        ");
		System.out.println("-----------     ---------------");
		System.out.println("| tp | fn |     | " + (int) tp + " | " + (int) fn + " |");
		System.out.println("----------- ==>	---------------");
		System.out.println("| fp | tn |     | " + (int) fp + " | " + (int) tn + " |");
		System.out.println("-----------     ---------------");
		float precision = tp / (tp + fp);
		float recall = tp / (tp + fn);
		System.out.println("Error Rate  : " + (fp + fn) / (tp + fn + fp + tn));
		System.out.println("Accuracy    : " + (tp + tn) / (tp + fn + fp + tn));
		System.out.println("Recall      : " + tp / (tp + fn));
		System.out.println("Precision   : " + tp / (tp + fp));
		System.out.println("F-1 measure : " + (2 * precision * recall) / (precision + recall));
		System.out.println();
	}

	public static void ClassifyTestExamplesBasedOnTrainingDataWithStopWords(boolean considerStopWords) {

		trueNegatives = 0;
		truePositives = 0;
		totalNoOfPositiveReviews = GetNoOfReviewsOfSpecificClass(new File(postiveTestReviewsDirectory));
		totalNoOfNegativeReviews = GetNoOfReviewsOfSpecificClass(new File(negativeTestReviewsDirectory));
		listTestFilesForFolder(new File(postiveTestReviewsDirectory), POSITIVE, considerStopWords);
		listTestFilesForFolder(new File(negativeTestReviewsDirectory), NEGATIVE, considerStopWords);
		System.out.println("Considering StopWords : " + considerStopWords);
		/*System.out.println("Correctly classified positive Test Reviews are : " + truePositives + " out of total "
				+ totalNoOfPositiveReviews);
		System.out.println("Correctly classified negative Test Reviews are : " + trueNegatives + " out of total "
				+ totalNoOfNegativeReviews);
		float accuracyPercentageForPositive = ((float) truePositives / (float) totalNoOfPositiveReviews) * 100;
		float accuracyPercentageForNegative = ((float) trueNegatives / (float) totalNoOfNegativeReviews) * 100;
		System.out.println("Percentage of Accuracy For positive results : " + accuracyPercentageForPositive);
		System.out.println("Percentage of Accuracy For positive results : " + accuracyPercentageForNegative);*/
		System.out.println();
	}

	public static void resetAllParams() {
		isBinaryNaiveBayes = true;
		ProbabilitesOfAllUniqueWordsMap.clear();
		allUniqueWordsMap.clear();
		probabilityOfPositive = BigDecimal.valueOf(0);
		probabilityOfnegative = BigDecimal.valueOf(0);
		truePositives = 0;
		trueNegatives = 0;
		totalNoOfPositiveReviews = 0;
		totalNoOfNegativeReviews = 0;
	}

	public static void main(String[] args) throws IOException {
		System.out.println("+++++++++++++++++++++++++++ NORMAL NAIVE BAYES +++++++++++++++++++++++++++++++++++++++++");
		StartAlgo(); // Normal Naive Bayes
		resetAllParams();
		System.out.println("+++++++++++++++++++++++++++ BINARY NAIVE BAYES +++++++++++++++++++++++++++++++++++++++++");
		StartAlgo(); // Binary Naive Bayes
		// printCountsMap(allUniqueWordsMap);
		// printProbabilitiesMap(ProbabilitesOfAllUniqueWordsMap);
		System.out.println("+++++++++++++++++++++++++++ END OF PROGRAM +++++++++++++++++++++++++++++++++++++++++++++");

	}

}
