import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class AadharFrequencyCountByGenderInState {
	public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable> {
		// The input key class is Object
		// The input value class is Text, we are handeling Text
		// The output key class is Text
		// The output value class is IntWritable
		private final static IntWritable one = new IntWritable(1);
		// This will be used to store the value of '1' with each word

		// This will be used to store the words as key/value pairs
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String valueString = value.toString();
			String[] aadharData = valueString.split(",");
			Text Key = new Text(aadharData[2] + "," + aadharData[6]);
			context.write(Key, one);
		}
	}

	// before reduce phase all the pairs with same keys are brought
	// together or they are shuffeled and arranged, so that they can be
	// worked upon as a group
	public static class IntSumReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
		// The input key class is Text (Each word acts as key)
		// The input value class is IntWritable, for each word 1
		// The output key class is Text
		// The output value class is IntWritable
		private IntWritable result = new IntWritable();

		// this is used to count the occurances of each word
		public void reduce(Text t_key, Iterable<IntWritable> values, Context context)
				throws IOException, InterruptedException {
			int aadharFrequencyForState = 0;
			 // initially sum is set as 0, before we start
			 // counting teh occurances
			 for (IntWritable val : values) {
				 aadharFrequencyForState += val.get();
			 // for each occurance we are adding the value (which is 1)
			 // so, in other words we are adding 1 for each occurance of a
			 // particular word
			 }
			 result.set(aadharFrequencyForState);
			 // putting the value of result as the number of occurances found
			 context.write(t_key, result);
			 // writing the result
		}
	}

	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "Finding Gender Wise Aadhar Count For Each State");
		job.setJarByClass(AadharFrequencyCountByGenderInState.class);
		job.setMapperClass(TokenizerMapper.class);
		job.setCombinerClass(IntSumReducer.class);
		job.setReducerClass(IntSumReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
