import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableUtils;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
public class AvgSalMF
{
public static class TokenizerMapper extends Mapper<LongWritable, Text, Text, FloatWritable> //op is text anf float writables 
                                                                                            //to   reduer input
{
	
	private static class CompositeGroupKey implements
	WritableComparable<CompositeGroupKey> {
	String country;
	String state;
	public void write(DataOutput out) throws IOException {
	WritableUtils.writeString(out, country);
	WritableUtils.writeString(out, state);
	}
	public void readFields(DataInput in) throws IOException {
	this.country = WritableUtils.readString(in);
	this.state = WritableUtils.readString(in);
	}
	public int compareTo(CompositeGroupKey pop) {
	if (pop == null)
	return 0;
	int intcnt = country.compareTo(pop.country);
	return intcnt == 0 ? state.compareTo(pop.state) : intcnt;
	}
	@Override
	public String toString() {
	return country.toString() + ":" + state.toString();
	}
	
	}
//private final static FloatWritable one = new FloatWritable(1);
//private FloatWritable sal = new FloatWritable(0);
public void map(LongWritable key, Text empRecord, Context context) throws IOException,InterruptedException {
		
			String[] word = empRecord.toString().split(" ");
			String SEX = word[1];
			  try {
			   Float Salary = Float.parseFloat(word[2]);
			   context.write(new Text(SEX), new FloatWritable(Salary));
			  } catch (Exception e) {
			   e.printStackTrace();
			  }
			}
			}

public static class FloatAvgSalReducer extends Reducer<Text, FloatWritable, Text, Text> {// output to client is text and text
public void reduce(Text key, Iterable<FloatWritable> values, Context context) throws IOException, InterruptedException
			{
			try{
					float total = (float) 0;
					int count = 0;
					for (FloatWritable var : values)
					{
					total += var.get();
					count++;
					} 
				float avg = (total/(float)count);
				String out = "Total: " + total + " :: " + "Average: " + avg;
				context.write(key, new Text(out));
	
				}	
				catch (Exception e) {
					e.printStackTrace();
					}
			}
}			
			
public static void main(String[] args) throws Exception {
Configuration conf = new Configuration();
Job job = Job.getInstance(conf, "avgsalmf");
job.setJarByClass(AvgSalMF.class);
job.setMapperClass(TokenizerMapper.class);
job.setReducerClass(FloatAvgSalReducer.class);
job.setMapOutputKeyClass(Text.class);
job.setMapOutputValueClass(FloatWritable.class);
job.setOutputKeyClass(Text.class);
job.setOutputValueClass(Text.class);
FileInputFormat.addInputPath(job, new Path(args[0]));
FileOutputFormat.setOutputPath(job, new Path(args[1]));
System.exit(job.waitForCompletion(true) ? 0 : 1);
}
}
