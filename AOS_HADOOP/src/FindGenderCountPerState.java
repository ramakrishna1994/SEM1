
/*
 * Authors : 1.Avinesh Benjamin (2017H1030080H)
 *                       2.Saradhi Ramakrishna (2017H1030081H)
 *                       3.Anmol Dayal Dhiman (2017H1030087H)
 * Branch : M.E Computer Science
 * Below code is practical implementation of State Wise and Gender wise Aadhar Application Enrolled count
 * using Hadoop Map-Reduce Jobs.
 * Input : Aadhar data in CSV format.
 * Output : Displays a list of statewise and genderwise aadhar enrollment count.
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableUtils;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class FindGenderCountPerState {

    private static class CompositeGroupKey implements WritableComparable<CompositeGroupKey> {
        String state;
        String gender;

        public CompositeGroupKey(String state, String gender) {
                this.state = state;
                this.gender = gender;
        }

        public void write(DataOutput out) throws IOException {
                WritableUtils.writeString(out, state);
                WritableUtils.writeString(out, gender);
        }

        public void readFields(DataInput in) throws IOException {
                this.state = WritableUtils.readString(in);
                this.gender = WritableUtils.readString(in);
        }

        public int compareTo(CompositeGroupKey pop) {
                if (pop == null)
                        return 0;
                int intcnt = state.compareTo(pop.state);
                return intcnt == 0 ? gender.compareTo(pop.gender) : intcnt;
        }

        @Override
        public String toString() {
                return state.toString() + " " + gender.toString() + " -> # ";
        }
    }

    public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable> {

        private final static IntWritable one = new IntWritable(1);

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
                String str = value.toString();
                String[] itr = str.split(",");
                CompositeGroupKey aadharData = null;
                if (!itr[0].equals("Registrar")) {
                        switch (itr[6]) {
                        case "M":
                                aadharData = new CompositeGroupKey(itr[2], "Male");
                                break;
                        case "F":
                                aadharData = new CompositeGroupKey(itr[2], "Female");
                                break;
                        }
                        context.write(new Text(aadharData.toString()), one);
                }
        }
    }

    public static class IntSumReducer extends Reducer<Text, IntWritable, Text, IntWritable> {

        private IntWritable result = new IntWritable();

        public void reduce(Text key, Iterable<IntWritable> values, Context context)
                        throws IOException, InterruptedException {
                int sum = 0;
                for (IntWritable val : values) {
                        sum += val.get();
                }
                result.set(sum);
                context.write(new Text(key.toString()), result);

        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Find Gender Count Per State ");
        job.setJarByClass(FindGenderCountPerState.class);
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
  