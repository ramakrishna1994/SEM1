/*
 * Authors : 1.Avinesh Benjamin (2017H1030080H)
 * 			 2.Saradhi Ramakrishna (2017H1030081H)
 * 			 3.Anmol Dayal Dhiman (2017H1030087H)
 * Branch : M.E Computer Science
 * Below code is practical implementation of State Wise Aadhar Application Rejected list
 * in Descending order using Hadoop Map-Reduce Jobs.
 * Input : Aadhar data in CSV format.
 * Output : Displays a list of statewise aadhar rejection list in descending order.
 * Note : We are using 2 map reduce jobs. First map reduce job output will be given to
 * 		second map reduce job which will write the output to HDFS.
 */

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

public class StateWiseReject {
	public static class MapperStage1 extends Mapper<Object, Text, Text, LongWritable> {

		private final static LongWritable one = new LongWritable(1);

		private Text word = new Text();

		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
			String str = value.toString();
			String[] itr = str.split(",");
			if (!itr[0].equals("Registrar")) {
				if (itr[9].equals("0")) {
					word.set(itr[2]+";");
					context.write(word, one);
				}
			}
		}
	}

	public static class ReducerStage1 extends Reducer<Text, LongWritable, Text, LongWritable> {
		private LongWritable result = new LongWritable();

		public void reduce(Text key, Iterable<LongWritable> values, Context context)
				throws IOException, InterruptedException {
			long sum = 0;
			for (LongWritable val : values) {
				sum += val.get();
			}
			result.set(sum);
			context.write(key, result);
		}
	}

	public static class MapperStage2 extends Mapper<Object , Text, LongWritable, Text>{
		
		public void map(Object key, Text values, Context context) throws IOException, InterruptedException {
			String []itr = values.toString().split(";");
			itr[0] = itr[0].trim();
			itr[1] = itr[1].trim();
			LongWritable k = new LongWritable(Integer.parseInt(itr[1]));
			context.write( k, new Text(itr[0]));
		}                  
	}
	
	public static class ReducerStage2 extends Reducer<LongWritable , Text, Text, LongWritable> {

		public void reduce(LongWritable key, Iterable<Text> values, Context context)
				throws IOException, InterruptedException {
			for(Text val : values) {
				context.write(val, key);
			}
		}
	}
	
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		
		Path file_path = new Path(args[1]);
		
		Job job1 = Job.getInstance(conf, "Count Rejected Applications");
		Job job2 = Job.getInstance(conf, "Sort in Descending Order");
		
		job1.setJarByClass(StateWiseReject.class);
		job1.setMapperClass(MapperStage1.class);
		job1.setCombinerClass(ReducerStage1.class);
		job1.setReducerClass(ReducerStage1.class);
		job1.setOutputKeyClass(Text.class);
		job1.setOutputValueClass(LongWritable.class);
		FileInputFormat.addInputPath(job1, new Path(args[0]));
		FileOutputFormat.setOutputPath(job1, new Path(file_path,"temp"));
		
		// If job 1 fails Exit program with code 1
		if(!job1.waitForCompletion(true)) {
			System.exit(1);
		}
		
		job2.setJarByClass(StateWiseReject.class);
		job2.setMapperClass(MapperStage2.class);
		job2.setMapOutputKeyClass(LongWritable.class); //output of Key and value during Map Phase
		job2.setMapOutputValueClass(Text.class);
		
		job2.setSortComparatorClass(LongWritable.DecreasingComparator.class); //Sorts In Descending Order
		job2.setReducerClass(ReducerStage2.class);

		job2.setOutputKeyClass(Text.class);
		job2.setOutputValueClass(LongWritable.class);
		
		FileInputFormat.addInputPath(job2, new Path(file_path, "temp"));
		FileOutputFormat.setOutputPath(job2, new Path(file_path, "out2"));
				
		System.exit(job2.waitForCompletion(true) ? 0 : 1);
	}
}
