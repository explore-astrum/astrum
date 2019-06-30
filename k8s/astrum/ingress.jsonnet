local kube = import '../kube.libsonnet';

kube.Ingress('ingress') {
  metadata: {
    name: 'ingress',
    namespace: 'astrum',
    annotations: {
      'nginx.ingress.kubernetes.io/ssl-redirect': 'false',
    },
  },
  spec: {
    rules: [
      {
        host: 'astrum.ironbay.co',
        http: {
          paths: [
            {
              path: '/',
              backend: {
                serviceName: 'elixir',
                servicePort: 12000,
              },
            },
          ],
        },
      },
    ],
  },
}
