void kernel update_ngg(global float *class, global float *sample, global uchar *flags,
			      unsigned int size, float l) {
    int gid = get_global_id(0);
    size_t i;
    size_t global_size = get_global_size(0);
    float new_weight, sample_weight, class_weight;
    uchar flag;

    for (i=gid; i<size; i+=global_size) {
        class_weight = class[i];
	sample_weight = sample[i];
	new_weight = class_weight + (sample_weight - class_weight) * l;
	class[i] = new_weight;
	flag = (uchar)(class_weight == 0 && sample_weight != 0);
	flags[i] = flag;
    }
}
