import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;



public class RejectedCount {
	
	public static class Mapper1 extends Mapper<Object, Text, Text, LongWritable>{
	// The input key class is Object 
	// The input value class is Text, we are handeling Text
	// The output key class is Text
	// The output value class is LongWritable


	private final static LongWritable one = new LongWritable(1);
	// This will be used to store the words as key/value pairs

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		 		
                        String line = value.toString();
                        String[] elements = line.split(",");


                        
                        String stateVar =elements[2];
                        int rej = Integer.parseInt(elements[8]);


                        //State stateObj = new state(stateVar,gender);
         				if(rej==0 && !stateVar.equals("Aadhar generated")){
         					
                        	context.write(new Text(stateVar),one);
         				}
                
                        
                    


	
	}
}

// before reduce phase all the pairs with same keys are brought
// together or they are shuffeled and arranged, so that they can be
// worked upon as a group
public static class Map1Reducer extends Reducer<Text,LongWritable, LongWritable, Text> {
		// The input key class is Teeext (Each word acts as key)
		// The input value class is LongWritable, for each word 1
		// The output key class is Text
		// The output value class is LongWritable

		private LongWritable result = new LongWritable();
		// this is used to count the occurances of each word

		public void reduce(Text key, Iterable<LongWritable> values,Context context) throws IOException, InterruptedException {
			int sum = 0;
			// initially sum is set as 0, before we start
			// counting teh occurances
			for (LongWritable val : values) {
				sum += val.get();
				// for each occurance we are adding the value (which is 1)
				// so, in other words we are adding 1 for each occurance of a
				// particular word
			}
				result.set(sum);
				// putting the value of result as the number of occurances found
				context.write(result, key);
				// writing the result
		}
	}
public static class Mapper2 extends Mapper<Object, Text, LongWritable, Text>{
	// The input key class is Object 
	// The input value class is Text, we are handeling Text
	// The output key class is Text
	// The output value class is LongWritable


	//private final static LongWritable one = new LongWritable(1);
	// This will be used to store the value of '1' with each word
	//private Text word = new Text();
	// This will be used to store the words as key/value pairs

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		 		
                        String line = value.toString();
                        String[] elements = line.split(",");


                        
                        String stateVar =elements[0];
                        //int rej = Integer.parseInt(elements[1]);

                        LongWritable rej = new LongWritable(Integer.parseInt(elements[1]));
                        //State stateObj = new state(stateVar,gender);
         				
                
                        context.write(rej , new Text(stateVar));
                    


	
	}
}

// before reduce phase all the pairs with same keys are brought
// together or they are shuffeled and arranged, so that they can be
// worked upon as a group
public static class Map2Reducer extends Reducer<Text,LongWritable,Text,LongWritable> {
		// The input key class is Teeext (Each word acts as key)
		// The input value class is LongWritable, for each word 1
		// The output key class is Text
		// The output value class is LongWritable

		private LongWritable result = new LongWritable();
		// this is used to count the occurances of each word

		public void reduce(LongWritable key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
			for(Text val : values)
			{
				context.write(val, key);
			}
		}
	}


private static final boolean True = false;


	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		Path path  = new Path(args[1]);	
		
		Job job1 = Job.getInstance(conf, "Rejected count Aadhar");
		Job job2 = Job.getInstance(conf,"print in descending order");
		job1.setJarByClass(RejectedCount.class);
		job1.setMapperClass(Mapper1.class);
		job1.setCombinerClass(Map1Reducer.class);
		job1.setReducerClass(Map1Reducer.class);
		job1.setOutputKeyClass(Text.class);
		job1.setOutputValueClass(LongWritable.class);
		FileInputFormat.addInputPath(job1, new Path(args[0]));
		FileOutputFormat.setOutputPath(job1, new Path(args[1]));
		
		System.exit(job1.waitForCompletion(true) ? 0 : 1);
		
		}
	}

