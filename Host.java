import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import java.lang.reflect.Field;
import sun.net.InetAddressCachePolicy;

public class Host {

    private static enum Input {
        count("-loop"), interval("-interval"), host("-hostname");
        String name;

        Input(String name) {
            this.name = name;
        }

        static Input valueof(String name) {
            switch (name) {
            case "-loop":
                return count;
            case "-interval":
                return interval;
            case "-hostname":
                return host;
            default:
                System.err.println(
                        String.format("only support input: %s(how many times to lookup the host), %s(waiting between each lookup)", count.name, interval.name));
                throw new RuntimeException("faiil to parse the input parameters " + name);
            }
        }
    }

    private static Map<Input, String> inputs = new HashMap<>();

    public static void main(String[] args) throws Throwable {
        // check if ipv6 supported
        Field ipv6Impl = InetAddress.class.getDeclaredField("impl");
        ipv6Impl.setAccessible(true);
        Object obj = ipv6Impl.get(InetAddress.class);
        boolean ipv6Support = obj.getClass().getName().equals("java.net.Inet6AddressImpl");
        System.out.println(System.getProperty("os.name") + " support " + (ipv6Support ? "ipv6" : "ipv4"));

        // modify dns cache time
        Field cache = sun.net.InetAddressCachePolicy.class.getDeclaredField("cachePolicy");
        cache.setAccessible(true);
        System.out.println("before change, dns cache is " + cache.get(sun.net.InetAddressCachePolicy.class) + "s");
        cache.set(sun.net.InetAddressCachePolicy.class, 0);
        System.out.println("after change, dns cache is " + cache.get(sun.net.InetAddressCachePolicy.class) + "s");

        // parse the input
        for (int i = 0; args != null && i < args.length; i++) {
            inputs.put(Input.valueof(args[i]), args[++i]);
        }
        String hostname = inputs.get(Input.host) == null ? "baidu.com" : inputs.get(Input.host);
        System.out.print("hostname is " + hostname);
        int count = inputs.get(Input.count) == null ? 20 : Integer.valueOf(inputs.get(Input.count));
        System.out.print("; count loop is " + count);
        int interval = inputs.get(Input.interval) == null ? 5 : Integer.valueOf(inputs.get(Input.interval));
        System.out.println("; every " + interval + " seconds");
        int i = 0;
        while (i++ < count) {
            System.out.println("----------");
            Arrays.asList(InetAddress.getAllByName(hostname)).forEach(System.out::println);
            TimeUnit.SECONDS.sleep(interval);
        }
    }
}
