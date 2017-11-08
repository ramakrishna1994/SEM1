import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.File;
import java.util.Iterator;  
//import org.apache.commons.io.FileUtils;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;

public class Adhar_1
{
	private static class CompositeGroupKey implements
	WritableComparable<CompositeGroupKey> {
	String state;
	String gender;
	 public CompositeGroupKey(String state, String gender) {
	     this.state = state;
	     this.gender = gender;
	 }
	public void write(DataOutput out) throws IOException {
	 //this.state.write(out);
    // this.gender.write(out);
	}
	public void readFields(DataInput in) throws IOException {
		//this.state.readFields(in);
	 //   this.gender.readFields(in);
	}
	public int compareTo(CompositeGroupKey pop) {
	if (pop == null)
	return 0;
	int intcnt = state.compareTo(pop.state);
	return intcnt == 0 ? gender.compareTo(pop.gender) : intcnt;
	}
	@Override
	public String toString() {
	return state.toString() + ":" + gender.toString();
	}
	}		
	
	public static class TokenizerMapper 
	extends Mapper<Object, Text, Text, 
	IntWritable>
	{
	private final static IntWritable one = new IntWritable(1);
	
	
	public void map(Object key, Text value, Context context) throws IOException, 
	InterruptedException
	{
	
		String[] word = value.toString().split(",");
		
		
		String state = word[2]; 
		
		String sex = word[6]; 
		
	//	String composite_key = state + "," + sex;
		CompositeGroupKey composite_key = new CompositeGroupKey(state, sex);
	
		context.write(new Text(composite_key.toString()), one); 
	
	}
	}
	public static class IntSumReducer extends Reducer<Text,IntWritable,Text,Text>
	{
	public void reduce(Text key, Iterable<IntWritable> values,Context context
	) throws IOException, InterruptedException
	{
	int sum=0,count=0;
	
	for (IntWritable val : values)
	{
	sum += val.get();
	count+=1;
	}
	

	String op="Total Applications are : " + sum;
	
	
	context.write(key, new Text(op));
	}
	}
	public static void main(String[] args) throws Exception
	{
	Configuration conf = new Configuration();
	Job job = Job.getInstance(conf, "Adhar1");
	
	job.setJarByClass(Adhar_1.class);
	job.setMapperClass(TokenizerMapper.class);
	job.setReducerClass(IntSumReducer.class);
	
	job.setMapOutputKeyClass(Text.class);
	job.setMapOutputValueClass(IntWritable.class);
	
	job.setOutputKeyClass(Text.class);
	job.setOutputValueClass(Text.class);
	
	FileInputFormat.addInputPath(job, new Path(args[0]));
	FileOutputFormat.setOutputPath(job, new Path(args[1]));
	System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
