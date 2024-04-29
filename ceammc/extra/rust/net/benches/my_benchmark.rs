use criterion::{black_box, criterion_group, criterion_main, Criterion};
// use net_rust::utils;

pub fn criterion_benchmark(c: &mut Criterion) {
    // c.bench_function("fib 20", |b| b.iter(|| 
    //     net_rust::utils::parse_ascii_filename(black_box("filename=\"abc.jpg\""))));
}

criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);
