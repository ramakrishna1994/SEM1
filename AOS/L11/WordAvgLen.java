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
public class WordAvgLen
{
public static class TokenizerMapper extends Mapper<Object, Text, IntWritable,
IntWritable>
{
private final static IntWritable one = new IntWritable(1);
private IntWritable len = new IntWritable(0);
public void map(Object key, Text value, Context context) throws IOException,
InterruptedException
{
StringTokenizer itr = new StringTokenizer(value.toString());
while (itr.hasMoreTokens())
{
len.set(itr.nextToken().length());
context.write(one,len);
}
}
}
public static class IntSumReducer extends Reducer<IntWritable,IntWritable,Text,Text>
{
public void reduce(IntWritable key, Iterable<IntWritable> values, Context context
) throws IOException, InterruptedException
{
int sum=0,count=0;
for (IntWritable val : values)
{
sum += val.get();
count+=1;
}
float avg=(sum/(float)count);
String op="Average length of " + count + " words = " + avg;
context.write(new Text(op), new Text(""));
}
}
public static void main(String[] args) throws Exception
{
Configuration conf = new Configuration();
Job job = Job.getInstance(conf, "wordLenAvg");
job.setJarByClass(WordAvgLen.class);
job.setMapperClass(TokenizerMapper.class);
job.setReducerClass(IntSumReducer.class);
job.setMapOutputKeyClass(IntWritable.class);
job.setMapOutputValueClass(IntWritable.class);
job.setOutputKeyClass(Text.class);
job.setOutputValueClass(Text.class);
FileInputFormat.addInputPath(job, new Path(args[0]));
FileOutputFormat.setOutputPath(job, new Path(args[1]));
System.exit(job.waitForCompletion(true) ? 0 : 1);
}
}
